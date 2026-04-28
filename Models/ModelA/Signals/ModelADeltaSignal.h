#pragma once

#include "DeltaSignal.h"

namespace dc {

struct ModelADeltaSignal : DeltaSignal {
    float temperature { 0.0f };
    float pressure    { 0.0f };
    float flowRate    { 0.0f };
    float voltage     { 0.0f };
};

} // NAMESPACE DC