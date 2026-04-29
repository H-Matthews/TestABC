#pragma once

#include "Signal.h"
#include <optional>

namespace dc {

struct ModelCDeltaSignal : sd::Signal {
    int numericIndex { 0 };

    std::optional<float> temperature;
    std::optional<float> pressure;
    std::optional<int>   rpm;
    std::optional<float> torque;
};

} // namespace dc
