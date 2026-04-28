#pragma once

#include "Signal.h"
#include <optional>

namespace dc {

// A partial update for a ModelA instance. Only populate the fields that have
// changed — unpopulated optionals are ignored during the merge.
struct ModelADeltaSignal : sd::Signal {
    int numericIndex;

    std::optional<float> temperature;
    std::optional<float> pressure;
    std::optional<float> flowRate;
    std::optional<float> voltage;
};

} // namespace dc
