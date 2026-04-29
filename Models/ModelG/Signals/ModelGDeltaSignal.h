#pragma once

#include "Signal.h"
#include <optional>

namespace dc {

struct ModelGDeltaSignal : sd::Signal {
    int numericIndex { 0 };

    std::optional<int>   rpm;
    std::optional<float> torque;
    std::optional<float> position;
    std::optional<float> velocity;
};

} // namespace dc
