#pragma once

#include "Signal.h"

namespace dc {

struct ModelBCommandSignal : sd::Signal {
    int numericIndex { 0 };
    int rpm          { 0 };
};

} // NAMESPACE DC