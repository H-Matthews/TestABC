#pragma once

#include <cstdint>

namespace dc {

struct ModelAState {
    float temperature { 0.0f };
    float pressure    { 0.0f };
    float flowRate    { 0.0f };
    float voltage     { 0.0f };
};
    
enum class ModelAField : uint32_t {
    Temperature = 1 << 0,
    Pressure    = 1 << 1,
    FlowRate    = 1 << 2,
    Voltage     = 1 << 3,
};

static constexpr uint32_t kModelAAllFields =
    static_cast<uint32_t>(ModelAField::Temperature) |
    static_cast<uint32_t>(ModelAField::Pressure)    |
    static_cast<uint32_t>(ModelAField::FlowRate)    |
    static_cast<uint32_t>(ModelAField::Voltage);

} // NAMESPACE DC