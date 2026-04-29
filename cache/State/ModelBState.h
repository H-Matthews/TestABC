#pragma once

#include "MechanicalState.h"

namespace dc {

struct ModelBState : MechanicalState {
    static bool isComplete(const ModelBState& s) {
        return s.rpm && s.torque;
    }
};

} // namespace dc
