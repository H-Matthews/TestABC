#pragma once

#include "PhysicsState.h"
#include <optional>

namespace dc {

struct ModelIState : PhysicsState {
    std::optional<float> density;
    std::optional<float> viscosity;

    void mergeFrom(const ModelIState& d) {
        if (d.temperature) temperature = d.temperature;
        if (d.pressure)    pressure    = d.pressure;
        if (d.density)     density     = d.density;
        if (d.viscosity)   viscosity   = d.viscosity;
    }

    static bool isComplete(const ModelIState& s) {
        return s.temperature && s.pressure && s.density && s.viscosity;
    }
};

} // namespace dc
