#pragma once

#include <optional>

namespace dc {

struct ModelBState {
    std::optional<int>   rpm;
    std::optional<float> torque;

    // Required fields — snapshot is published once all of these are populated.
    // To make a field optional, simply omit it from this check.
    static bool isComplete(const ModelBState& s) {
        return s.rpm && s.torque;
    }
};

} // namespace dc
