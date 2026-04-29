#pragma once

#include "Signal.h"

namespace dc {

struct ModelHCommandSignal : sd::Signal {
    int numericIndex { 0 };
    int rpm          { 0 };
};

} // namespace dc
