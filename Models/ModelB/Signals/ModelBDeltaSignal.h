#pragma once

#include "Signal.h"
#include <optional>

namespace dc {

// A partial update for a ModelB instance. Only populate the fields that have
// changed — unpopulated optionals are ignored during the merge.
struct ModelBDeltaSignal : sd::Signal {
    int numericIndex;

    std::optional<int>   rpm;
    std::optional<float> torque;
};

} // namespace dc
