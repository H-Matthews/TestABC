#pragma once

#include <optional>

namespace dc {

struct PhysicsState {
    std::optional<float> temperature;
    std::optional<float> pressure;
};

} // namespace dc
