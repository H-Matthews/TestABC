#pragma once

#include "PhysicsState.h"
#include "ElectricalState.h"
#include <optional>

namespace dc {

struct ModelAState : PhysicsState, ElectricalState {
    std::optional<float> flowRate;

    void mergeFrom(const ModelAState& d) {
        if (d.temperature) temperature = d.temperature;
        if (d.pressure)    pressure    = d.pressure;
        if (d.voltage)     voltage     = d.voltage;
        if (d.flowRate)    flowRate    = d.flowRate;
    }

    static bool isComplete(const ModelAState& s) {
        return s.temperature && s.pressure && s.voltage && s.flowRate;
    }
};

} // namespace dc
