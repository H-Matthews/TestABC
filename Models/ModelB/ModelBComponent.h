#pragma once

#include "SignalDispatcher.h"
#include "ModelBDeltaSignal.h"
#include "ModelBCommandSignal.h"

#include <iostream>
#include <optional>
#include <string>

namespace dc {

class ModelBComponent {
public:
    ModelBComponent(sd::SignalDispatcher& dispatcher, int index)
        : dispatcher_(dispatcher)
        , index_(index)
        , alias_("model_b." + std::to_string(index))
    {
        dispatcher_.bind<ModelBCommandSignal>(
            alias_, alias_, this, &ModelBComponent::onCommand);
    }

    ~ModelBComponent() {
        dispatcher_.disconnect(alias_);
    }

    void broadcastDelta(std::optional<int> rpm, std::optional<float> torque) {
        ModelBDeltaSignal signal;
        signal.numericIndex = index_;
        signal.rpm          = rpm;
        signal.torque       = torque;
        dispatcher_.broadcast(alias_, signal);
    }

private:
    void onCommand(const ModelBCommandSignal& signal) {
        std::cout << "[ModelB " << index_ << "] command received:"
                  << " rpm=" << signal.rpm << "\n";
    }

    sd::SignalDispatcher& dispatcher_;
    int         index_;
    std::string alias_;
};

} // NAMESPACE DC