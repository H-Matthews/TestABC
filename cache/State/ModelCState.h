#pragma once

#include "PhysicsState.h"
#include "MechanicalState.h"

namespace dc {

struct ModelCState : PhysicsState, MechanicalState {
    void mergeFrom(const ModelCState& d) {
        if (d.temperature) temperature = d.temperature;
        if (d.pressure)    pressure    = d.pressure;
        if (d.rpm)         rpm         = d.rpm;
        if (d.torque)      torque      = d.torque;
    }

    static bool isComplete(const ModelCState& s) {
        return s.temperature && s.pressure && s.rpm && s.torque;
    }
};

} // namespace dc
