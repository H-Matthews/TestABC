#pragma once

#include "PhysicsState.h"
#include "ElectricalState.h"
#include "MechanicalState.h"

namespace dc {

struct ModelDState : PhysicsState, ElectricalState, MechanicalState {
    static bool isComplete(const ModelDState& s) {
        return s.temperature && s.pressure && s.voltage && s.rpm && s.torque;
    }
};

} // namespace dc
