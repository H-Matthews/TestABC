#pragma once

#include <optional>

namespace dc {

struct ModelAState {
    std::optional<float> temperature;
    std::optional<float> pressure;
    std::optional<float> flowRate;
    std::optional<float> voltage;

    // Required fields — snapshot is published once all of these are populated.
    // To make a field optional, simply omit it from this check.
    static bool isComplete(const ModelAState& s) {
        return s.temperature && s.pressure && s.flowRate && s.voltage;
    }
};

} // namespace dc
