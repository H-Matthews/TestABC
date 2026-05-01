#pragma once

#include "SignalDispatcher.h"
#include "DeltaSignal.h"
#include "ModelGState.h"
#include "ModelGCommandSignal.h"

#include <iostream>
#include <optional>
#include <string>

namespace dc {

class ModelGComponent {
public:
    ModelGComponent(sd::SignalDispatcher& dispatcher, int index)
        : dispatcher_(dispatcher)
        , index_(index)
        , alias_("model_g." + std::to_string(index))
    {
        dispatcher_.bind<ModelGCommandSignal>(alias_, alias_, [this](const ModelGCommandSignal& s) { onCommand(s); });
    }

    ~ModelGComponent() {
        dispatcher_.disconnect(alias_);
    }

    void broadcastDelta(std::optional<int> rpm,    std::optional<float> torque,
                        std::optional<float> position, std::optional<float> velocity)
    {
        DeltaSignal<ModelGState> signal;
        signal.numericIndex = index_;
        signal.rpm          = rpm;
        signal.torque       = torque;
        signal.position     = position;
        signal.velocity     = velocity;
        dispatcher_.broadcast(alias_, signal);
    }

private:
    void onCommand(const ModelGCommandSignal& signal) {
        std::cout << "[ModelG " << index_ << "] command received:"
                  << " position=" << signal.position << "\n";
    }

    sd::SignalDispatcher& dispatcher_;
    int         index_;
    std::string alias_;
};

} // namespace dc
