#pragma once

#include "SignalDispatcher.h"
#include "ModelHDeltaSignal.h"
#include "ModelHCommandSignal.h"

#include <iostream>
#include <optional>
#include <string>

namespace dc {

class ModelHComponent {
public:
    ModelHComponent(sd::SignalDispatcher& dispatcher, int index)
        : dispatcher_(dispatcher)
        , index_(index)
        , alias_("model_h." + std::to_string(index))
    {
        dispatcher_.bind<ModelHCommandSignal>(alias_, alias_, this, &ModelHComponent::onCommand);
    }

    ~ModelHComponent() {
        dispatcher_.disconnect(alias_);
    }

    void broadcastDelta(std::optional<float> voltage,
                        std::optional<int> rpm, std::optional<float> torque)
    {
        ModelHDeltaSignal signal;
        signal.numericIndex = index_;
        signal.voltage      = voltage;
        signal.rpm          = rpm;
        signal.torque       = torque;
        dispatcher_.broadcast(alias_, signal);
    }

private:
    void onCommand(const ModelHCommandSignal& signal) {
        std::cout << "[ModelH " << index_ << "] command received:"
                  << " rpm=" << signal.rpm << "\n";
    }

    sd::SignalDispatcher& dispatcher_;
    int         index_;
    std::string alias_;
};

} // namespace dc
