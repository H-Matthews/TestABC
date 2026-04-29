#pragma once

#include "PhysicsState.h"
#include "ElectricalState.h"
#include <optional>

namespace dc {

struct ModelAState : PhysicsState, ElectricalState {
    std::optional<float> flowRate;

    static bool isComplete(const ModelAState& s) {
        return s.temperature && s.pressure && s.voltage && s.flowRate;
    }
};

} // namespace dc
