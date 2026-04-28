#pragma once

#include <mutex>
#include <memory>
#include <cstdint>
#include <utility>

namespace dc {

template<typename TState>
struct InstanceRecord {
    int            numericIndex  { 0 };
    mutable std::mutex instanceMutex;
    TState         state         {};
    uint32_t       seenFields    { 0 };
    bool           ready         { false };

    // Acquires instanceMutex, applies field updates via applyFn, updates seenFields,
    // sets ready once all expected fields have been seen. Returns ready status.
    template<typename TDeltaSignal, typename TApplyFn>
    bool applyDelta(const TDeltaSignal& signal, uint32_t allFields, TApplyFn&& applyFn) {
        std::lock_guard lock(instanceMutex);

        // Converts back to an RValue and calls lambda
        std::forward<TApplyFn>(applyFn)(state, signal);
        seenFields |= signal.presentFields;
        if (!ready && (seenFields & allFields) == allFields) {
            ready = true;
        }
        return ready;
    }

    // Acquires instanceMutex, copies state, releases mutex, then allocates.
    // The allocation happens outside the lock to minimise contention.
    std::shared_ptr<const TState> snapshot() const {
        TState copy;
        {
            std::lock_guard lock(instanceMutex);
            copy = state;
        }
        return std::make_shared<const TState>(std::move(copy));
    }

    // Atomically checks ready and, if true, copies state under the same lock.
    // Allocation happens outside the lock. Returns {false, nullptr} if not ready.
    std::pair<bool, std::shared_ptr<const TState>> trySnapshot() const {
        TState copy;
        {
            std::lock_guard lock(instanceMutex);
            if (!ready) {
                return { false, nullptr };
            }
            copy = state;
        }
        return { true, std::make_shared<const TState>(std::move(copy)) };
    }
};

} // namespace dc
