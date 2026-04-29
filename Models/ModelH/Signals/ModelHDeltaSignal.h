#pragma once

#include "Signal.h"
#include <optional>

namespace dc {

struct ModelHDeltaSignal : sd::Signal {
    int numericIndex { 0 };

    std::optional<float> voltage;
    std::optional<int>   rpm;
    std::optional<float> torque;
};

} // namespace dc
