#pragma once

#include "ElectricalState.h"
#include "MechanicalState.h"

namespace dc {

struct ModelHState : ElectricalState, MechanicalState {
    static bool isComplete(const ModelHState& s) {
        return s.voltage && s.rpm && s.torque;
    }
};

} // namespace dc
