#pragma once

#include "ElectricalState.h"
#include <optional>

namespace dc {

struct ModelEState : ElectricalState {
    std::optional<float> current;
    std::optional<float> frequency;

    void mergeFrom(const ModelEState& d) {
        if (d.voltage)    voltage    = d.voltage;
        if (d.current)    current    = d.current;
        if (d.frequency)  frequency  = d.frequency;
    }

    static bool isComplete(const ModelEState& s) {
        return s.voltage && s.current && s.frequency;
    }
};

} // namespace dc
