#pragma once

#include "Signal.h"

namespace dc {

struct ModelACommandSignal : sd::Signal {
    int   numericIndex { 0 };
    float temperature  { 0.0f };
};

} // NAMESPACE DC