#pragma once

#include <optional>

namespace dc {

struct MechanicalState {
    std::optional<int>   rpm;
    std::optional<float> torque;
};

} // namespace dc
