#pragma once

#include "MechanicalState.h"

namespace dc {

struct ModelBState : MechanicalState {
    void mergeFrom(const ModelBState& d) {
        if (d.rpm)    rpm    = d.rpm;
        if (d.torque) torque = d.torque;
    }

    static bool isComplete(const ModelBState& s) {
        return s.rpm && s.torque;
    }
};

} // namespace dc
