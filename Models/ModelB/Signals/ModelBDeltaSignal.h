#pragma once

#include "DeltaSignal.h"

namespace dc {

struct ModelBDeltaSignal : DeltaSignal {
    int   rpm   { 0 };
    float torque { 0.0f };
};

} // NAMESPACE DC