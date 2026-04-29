#pragma once

#include "Signal.h"

namespace dc {

struct ModelGCommandSignal : sd::Signal {
    int   numericIndex { 0 };
    float position     { 0.0f };
};

} // namespace dc
