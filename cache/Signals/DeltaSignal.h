#pragma once

#include "Signal.h"

namespace dc {

template<typename TState>
struct DeltaSignal : sd::Signal, TState {
    int numericIndex = -1;
};

} // namespace dc
