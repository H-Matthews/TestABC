#pragma once

#include "ModelAState.h"
#include "ModelBState.h"
#include "ModelCState.h"
#include "ModelDState.h"
#include "ModelEState.h"
#include "ModelFState.h"
#include "ModelGState.h"
#include "ModelHState.h"
#include "ModelIState.h"
#include "DeltaSignal.h"
#include "QueryResult.h"
#include "InstanceRecord.h"
#include "SignalDispatcher.h"

#include <functional>
#include <iostream>
#include <memory>
#include <shared_mutex>
#include <tuple>
#include <typeindex>
#include <unordered_map>

namespace dc {

// ---------------------------------------------------------------------------
// TypeStore<TState>
//
// Owns the per-type record map and its shared_mutex. Bundled into a single
// struct so it can be stored as a tuple element without extra bookkeeping.
// ---------------------------------------------------------------------------
template<typename TState>
struct TypeStore {
    std::unordered_map<int, std::unique_ptr<InstanceRecord<TState>>> records;
    mutable std::shared_mutex mutex;
};

// ---------------------------------------------------------------------------
// DataCache
// ---------------------------------------------------------------------------
class DataCache {
public:
    DataCache() = default;
    ~DataCache();

    DataCache(const DataCache&)            = delete;
    DataCache& operator=(const DataCache&) = delete;

    // Register a typed callback invoked when a model instance snapshot is ready.
    template<typename TState>
    void setCallback(std::function<void(int, std::shared_ptr<const TState>)> callback) {
        std::unique_lock lock(callbacksMutex_);
        callbacks_[std::type_index(typeid(TState))] =
            [cb = std::move(callback)](int index, std::shared_ptr<const void> snap) {
                cb(index, std::static_pointer_cast<const TState>(snap));
            };
    }

    // Pre-allocate instanceCount InstanceRecords for TState (indices 0..instanceCount-1).
    // One-shot per type: a second call logs a warning and returns early.
    // Must be called before simulation begins.
    template<typename TState>
    void reserve(int instanceCount) {
        auto key = std::type_index(typeid(TState));
        auto& flag = reserveFlags_[key];
        if (flag) {
            std::cerr << "[DataCache] Warning: reserve() called more than once for "
                      << typeid(TState).name() << "\n";
            return;
        }
        flag = true;

        auto& store = storeFor<TState>();
        std::unique_lock lock(store.mutex);
        store.records.reserve(static_cast<std::size_t>(instanceCount));
        for (int i = 0; i < instanceCount; ++i) {
            auto record = std::make_unique<InstanceRecord<TState>>();
            record->numericIndex = i;
            store.records.emplace(i, std::move(record));
        }
    }

    // Dynamically add a single InstanceRecord for TState at runtime.
    // Safe to call after simulation has started. No-op if the index already exists.
    template<typename TState>
    void addInstance(int index) {
        auto& store = storeFor<TState>();
        std::unique_lock lock(store.mutex);
        if (store.records.count(index)) return;
        auto record = std::make_unique<InstanceRecord<TState>>();
        record->numericIndex = index;
        store.records.emplace(index, std::move(record));
    }

    // Bind delta signal handlers for all model types. Stores a dispatcher pointer
    // for cleanup in the destructor. Call once at startup before signals flow.
    void registerHandlers(sd::SignalDispatcher& dispatcher);

    // Direct query path — bypasses the SignalDispatcher entirely.
    template<typename TState>
    QueryResult<TState> query(int numericIndex) const {
        const auto& store = storeFor<TState>();
        const InstanceRecord<TState>* record = nullptr;
        {
            std::shared_lock lock(store.mutex);
            auto it = store.records.find(numericIndex);
            if (it == store.records.end()) {
                std::cerr << "[DataCache] Warning: query() unknown index " << numericIndex
                          << " for " << typeid(TState).name() << "\n";
                return { QueryResult<TState>::Status::UnknownIndex, nullptr };
            }
            record = it->second.get();
        }

        auto [isReady, snap] = record->trySnapshot();
        if (!isReady) {
            return { QueryResult<TState>::Status::NotReady, nullptr };
        }
        return { QueryResult<TState>::Status::Ready, std::move(snap) };
    }

private:
    // Single generic delta handler — shared by all model types.
    template<typename TState>
    void onDelta(const DeltaSignal<TState>& signal) {
        auto& store = storeFor<TState>();
        InstanceRecord<TState>* record = nullptr;
        {
            std::shared_lock lock(store.mutex);
            auto it = store.records.find(signal.numericIndex);
            if (it == store.records.end()) {
                std::cerr << "[DataCache] Warning: " << typeid(TState).name()
                          << " delta for unknown index " << signal.numericIndex << "\n";
                return;
            }
            record = it->second.get();
        }

        bool ready = record->applyDelta(signal);
        if (ready) {
            publish<TState>(signal.numericIndex, record->snapshot());
        }
    }

    // Invoke the registered callback for TState with the given snapshot.
    template<typename TState>
    void publish(int index, std::shared_ptr<const TState> snapshot) {
        std::shared_lock lock(callbacksMutex_);
        auto it = callbacks_.find(std::type_index(typeid(TState)));
        if (it != callbacks_.end()) {
            it->second(index, std::static_pointer_cast<const void>(std::move(snapshot)));
        }
    }

    // Delta signal handlers — registered with the dispatcher as member function pointers.
    void onModelADelta(const DeltaSignal<ModelAState>& signal);
    void onModelBDelta(const DeltaSignal<ModelBState>& signal);
    void onModelCDelta(const DeltaSignal<ModelCState>& signal);
    void onModelDDelta(const DeltaSignal<ModelDState>& signal);
    void onModelEDelta(const DeltaSignal<ModelEState>& signal);
    void onModelFDelta(const DeltaSignal<ModelFState>& signal);
    void onModelGDelta(const DeltaSignal<ModelGState>& signal);
    void onModelHDelta(const DeltaSignal<ModelHState>& signal);
    void onModelIDelta(const DeltaSignal<ModelIState>& signal);

    // Typed accessor into the tuple — returns the TypeStore for TState.
    template<typename TState>
    TypeStore<TState>& storeFor() {
        return std::get<TypeStore<TState>>(stores_);
    }

    template<typename TState>
    const TypeStore<TState>& storeFor() const {
        return std::get<TypeStore<TState>>(stores_);
    }

    // One TypeStore per supported state type. Adding a new type means adding it
    // here and binding its handler in registerHandlers() — no other changes needed.
    std::tuple<
        TypeStore<ModelAState>,
        TypeStore<ModelBState>,
        TypeStore<ModelCState>,
        TypeStore<ModelDState>,
        TypeStore<ModelEState>,
        TypeStore<ModelFState>,
        TypeStore<ModelGState>,
        TypeStore<ModelHState>,
        TypeStore<ModelIState>
    > stores_;

    // Type-erased callbacks, keyed by state type.
    std::unordered_map<std::type_index,
        std::function<void(int, std::shared_ptr<const void>)>> callbacks_;
    mutable std::shared_mutex callbacksMutex_;

    std::unordered_map<std::type_index, bool> reserveFlags_;

    sd::SignalDispatcher* dispatcher_ { nullptr };
};

} // namespace dc
