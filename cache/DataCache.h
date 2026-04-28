#pragma once

#include "ModelAState.h"
#include "ModelBState.h"
#include "ModelADeltaSignal.h"
#include "ModelBDeltaSignal.h"
#include "QueryResult.h"
#include "InstanceRecord.h"
#include "SignalDispatcher.h"

#include <functional>
#include <memory>
#include <shared_mutex>
#include <typeindex>
#include <unordered_map>

namespace dc {

class DataCache {
public:
    DataCache() = default;
    ~DataCache();

    DataCache(const DataCache&)            = delete;
    DataCache& operator=(const DataCache&) = delete;

    // Register a typed callback invoked when a model instance snapshot is ready.
    // Must be called before registerHandlers().
    template<typename TState>
    void setCallback(std::function<void(int, std::shared_ptr<const TState>)> callback) {
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

        auto& map = recordMap<TState>();
        auto& mtx = recordMutex<TState>();

        std::unique_lock lock(mtx);
        map.reserve(static_cast<std::size_t>(instanceCount));
        for (int i = 0; i < instanceCount; ++i) {
            auto record = std::make_unique<InstanceRecord<TState>>();
            record->numericIndex = i;
            map.emplace(i, std::move(record));
        }
    }

    // Dynamically add a single InstanceRecord for TState at runtime.
    // Safe to call after simulation has started. No-op if the index already exists.
    template<typename TState>
    void addInstance(int index) {
        auto& map = recordMap<TState>();
        auto& mtx = recordMutex<TState>();

        std::unique_lock lock(mtx);
        if (map.count(index)) return;
        auto record = std::make_unique<InstanceRecord<TState>>();
        record->numericIndex = index;
        map.emplace(index, std::move(record));
    }

    // Bind delta signal handlers for all model types. Stores a dispatcher pointer
    // for cleanup in the destructor. Call once at startup before signals flow.
    void registerHandlers(sd::SignalDispatcher& dispatcher);

    // Direct query path — bypasses the SignalDispatcher entirely.
    template<typename TState>
    QueryResult<TState> query(int numericIndex) const {
        const InstanceRecord<TState>* record = nullptr;
        {
            std::shared_lock lock(recordMutex<TState>());
            const auto& map = recordMap<TState>();
            auto it = map.find(numericIndex);
            if (it == map.end()) {
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
    // Delta signal handlers — defined in DataCache.cpp.
    void onModelADelta(const ModelADeltaSignal& signal);
    void onModelBDelta(const ModelBDeltaSignal& signal);

    // Invoke the registered callback for TState with the given snapshot.
    template<typename TState>
    void publish(int index, std::shared_ptr<const TState> snapshot) {
        auto it = callbacks_.find(std::type_index(typeid(TState)));
        if (it != callbacks_.end()) {
            it->second(index, std::static_pointer_cast<const void>(std::move(snapshot)));
        }
    }

    // Route to the correct per-type record map using if constexpr (C++17).
    template<typename TState>
    auto& recordMap() {
        if constexpr (std::is_same_v<TState, ModelAState>) return modelARecords_;
        else                                                return modelBRecords_;
    }

    template<typename TState>
    const auto& recordMap() const {
        if constexpr (std::is_same_v<TState, ModelAState>) return modelARecords_;
        else                                                return modelBRecords_;
    }

    template<typename TState>
    std::shared_mutex& recordMutex() const {
        if constexpr (std::is_same_v<TState, ModelAState>) return modelAMutex_;
        else                                                return modelBMutex_;
    }

    std::unordered_map<int, std::unique_ptr<InstanceRecord<ModelAState>>> modelARecords_;
    std::unordered_map<int, std::unique_ptr<InstanceRecord<ModelBState>>> modelBRecords_;

    mutable std::shared_mutex modelAMutex_;
    mutable std::shared_mutex modelBMutex_;

    // Stores types as the erased state type so that we can store everything in this data structure.
    std::unordered_map<std::type_index,
        std::function<void(int, std::shared_ptr<const void>)>> callbacks_;

    std::unordered_map<std::type_index, bool> reserveFlags_;

    sd::SignalDispatcher* dispatcher_ { nullptr };
};

} // namespace dc
