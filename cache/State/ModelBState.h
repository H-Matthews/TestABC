#pragma once

#include <cstdint>

namespace dc {

struct ModelBState {
    int   rpm    { 0 };
    float torque { 0.0f };
};

enum class ModelBField : uint32_t {
    Rpm    = 1 << 0,
    Torque = 1 << 1,
};

static constexpr uint32_t kModelBAllFields =
    static_cast<uint32_t>(ModelBField::Rpm)    |
    static_cast<uint32_t>(ModelBField::Torque);

} // NAMESPACE DC