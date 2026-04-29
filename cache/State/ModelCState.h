#pragma once

#include "PhysicsState.h"
#include "MechanicalState.h"

namespace dc {

struct ModelCState : PhysicsState, MechanicalState {
    static bool isComplete(const ModelCState& s) {
        return s.temperature && s.pressure && s.rpm && s.torque;
    }
};

} // namespace dc
