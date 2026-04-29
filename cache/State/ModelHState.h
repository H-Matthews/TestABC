#pragma once

#include "ElectricalState.h"
#include "MechanicalState.h"

namespace dc {

struct ModelHState : ElectricalState, MechanicalState {
    void mergeFrom(const ModelHState& d) {
        if (d.voltage) voltage = d.voltage;
        if (d.rpm)     rpm     = d.rpm;
        if (d.torque)  torque  = d.torque;
    }

    static bool isComplete(const ModelHState& s) {
        return s.voltage && s.rpm && s.torque;
    }
};

} // namespace dc
