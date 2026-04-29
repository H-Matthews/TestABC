#pragma once

#include "Signal.h"

namespace dc {

struct ModelFCommandSignal : sd::Signal {
    int   numericIndex { 0 };
    float temperature  { 0.0f };
};

} // namespace dc
