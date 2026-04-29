#pragma once

#include "Signal.h"
#include <optional>

namespace dc {

struct ModelIDeltaSignal : sd::Signal {
    int numericIndex { 0 };

    std::optional<float> temperature;
    std::optional<float> pressure;
    std::optional<float> density;
    std::optional<float> viscosity;
};

} // namespace dc
