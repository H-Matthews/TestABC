#pragma once

#include "DeltaSignal.h"
#include <mutex>
#include <memory>
#include <utility>

namespace dc {

template<typename TState>
struct InstanceRecord {
    int            numericIndex  { 0 };
    mutable std::mutex instanceMutex;
    TState         state         {};
    bool           ready         { false };

    // Acquires instanceMutex, merges populated fields, then checks
    // TState::isComplete to determine readiness. Returns ready status.
    bool applyDelta(const DeltaSignal<TState>& signal) {
        std::lock_guard lock(instanceMutex);
        state.mergeFrom(signal);
        if (!ready) {
            ready = TState::isComplete(state);
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
