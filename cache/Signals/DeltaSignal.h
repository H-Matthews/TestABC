#pragma once

#include "Signal.h"

#include <cstdint>

namespace dc {

struct DeltaSignal : sd::Signal {
    int      numericIndex  { 0 };
    uint32_t presentFields { 0 };
};

} // namespace dc
