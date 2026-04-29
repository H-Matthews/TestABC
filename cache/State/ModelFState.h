#pragma once

#include "PhysicsState.h"
#include <optional>

namespace dc {

struct ModelFState : PhysicsState {
    std::optional<float> humidity;
    std::optional<float> flowRate;

    static bool isComplete(const ModelFState& s) {
        return s.temperature && s.pressure && s.humidity && s.flowRate;
    }
};

} // namespace dc
