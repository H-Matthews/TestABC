#pragma once

#include "Signal.h"
#include <optional>

namespace dc {

struct ModelFDeltaSignal : sd::Signal {
    int numericIndex { 0 };

    std::optional<float> temperature;
    std::optional<float> pressure;
    std::optional<float> humidity;
    std::optional<float> flowRate;
};

} // namespace dc
