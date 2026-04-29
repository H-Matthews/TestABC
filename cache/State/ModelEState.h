#pragma once

#include "ElectricalState.h"
#include <optional>

namespace dc {

struct ModelEState : ElectricalState {
    std::optional<float> current;
    std::optional<float> frequency;

    static bool isComplete(const ModelEState& s) {
        return s.voltage && s.current && s.frequency;
    }
};

} // namespace dc
