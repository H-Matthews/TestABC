#pragma once

#include "MechanicalState.h"
#include <optional>

namespace dc {

struct ModelGState : MechanicalState {
    std::optional<float> position;
    std::optional<float> velocity;

    static bool isComplete(const ModelGState& s) {
        return s.rpm && s.torque && s.position && s.velocity;
    }
};

} // namespace dc
