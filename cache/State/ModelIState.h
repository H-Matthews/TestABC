#pragma once

#include "PhysicsState.h"
#include <optional>

namespace dc {

struct ModelIState : PhysicsState {
    std::optional<float> density;
    std::optional<float> viscosity;

    static bool isComplete(const ModelIState& s) {
        return s.temperature && s.pressure && s.density && s.viscosity;
    }
};

} // namespace dc
