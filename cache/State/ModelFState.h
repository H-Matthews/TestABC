#pragma once

#include "PhysicsState.h"
#include <optional>

namespace dc {

struct ModelFState : PhysicsState {
    std::optional<float> humidity;
    std::optional<float> flowRate;

    void mergeFrom(const ModelFState& d) {
        if (d.temperature) temperature = d.temperature;
        if (d.pressure)    pressure    = d.pressure;
        if (d.humidity)    humidity    = d.humidity;
        if (d.flowRate)    flowRate    = d.flowRate;
    }

    static bool isComplete(const ModelFState& s) {
        return s.temperature && s.pressure && s.humidity && s.flowRate;
    }
};

} // namespace dc
