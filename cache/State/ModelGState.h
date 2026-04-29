#pragma once

#include "MechanicalState.h"
#include <optional>

namespace dc {

struct ModelGState : MechanicalState {
    std::optional<float> position;
    std::optional<float> velocity;

    void mergeFrom(const ModelGState& d) {
        if (d.rpm)      rpm      = d.rpm;
        if (d.torque)   torque   = d.torque;
        if (d.position) position = d.position;
        if (d.velocity) velocity = d.velocity;
    }

    static bool isComplete(const ModelGState& s) {
        return s.rpm && s.torque && s.position && s.velocity;
    }
};

} // namespace dc
