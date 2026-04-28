#pragma once

#include "Signal.h"
#include "Models.h"
#include <cstdint>

namespace dc {

struct ModelADeltaSignal : sd::Signal {
    int      numericIndex  { 0 };
    uint32_t presentFields { 0 };
    float    temperature   { 0.0f };
    float    pressure      { 0.0f };
    float    flowRate      { 0.0f };
    float    voltage       { 0.0f };
};

struct ModelBDeltaSignal : sd::Signal {
    int      numericIndex  { 0 };
    uint32_t presentFields { 0 };
    int      rpm           { 0 };
    float    torque        { 0.0f };
};

struct ModelACommandSignal : sd::Signal {
    int   numericIndex { 0 };
    float temperature  { 0.0f };
};

struct ModelBCommandSignal : sd::Signal {
    int numericIndex { 0 };
    int rpm          { 0 };
};

} // namespace dc
