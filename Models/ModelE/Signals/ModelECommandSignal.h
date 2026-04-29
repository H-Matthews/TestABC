#pragma once

#include "Signal.h"

namespace dc {

struct ModelECommandSignal : sd::Signal {
    int   numericIndex { 0 };
    float voltage      { 0.0f };
};

} // namespace dc
