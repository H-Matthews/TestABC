#pragma once

#include "PhysicsState.h"
#include "ElectricalState.h"
#include "MechanicalState.h"

namespace dc {

struct ModelDState : PhysicsState, ElectricalState, MechanicalState {
    void mergeFrom(const ModelDState& d) {
        if (d.temperature) temperature = d.temperature;
        if (d.pressure)    pressure    = d.pressure;
        if (d.voltage)     voltage     = d.voltage;
        if (d.rpm)         rpm         = d.rpm;
        if (d.torque)      torque      = d.torque;
    }

    static bool isComplete(const ModelDState& s) {
        return s.temperature && s.pressure && s.voltage && s.rpm && s.torque;
    }
};

} // namespace dc
