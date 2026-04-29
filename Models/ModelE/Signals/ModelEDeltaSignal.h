#pragma once

#include "Signal.h"
#include <optional>

namespace dc {

struct ModelEDeltaSignal : sd::Signal {
    int numericIndex { 0 };

    std::optional<float> voltage;
    std::optional<float> current;
    std::optional<float> frequency;
};

} // namespace dc
