#pragma once

#include "SignalDispatcher.h"
#include "ModelDDeltaSignal.h"
#include "ModelDCommandSignal.h"

#include <iostream>
#include <optional>
#include <string>

namespace dc {

class ModelDComponent {
public:
    ModelDComponent(sd::SignalDispatcher& dispatcher, int index)
        : dispatcher_(dispatcher)
        , index_(index)
        , alias_("model_d." + std::to_string(index))
    {
        dispatcher_.bind<ModelDCommandSignal>(alias_, alias_, this, &ModelDComponent::onCommand);
    }

    ~ModelDComponent() {
        dispatcher_.disconnect(alias_);
    }

    void broadcastDelta(std::optional<float> temperature, std::optional<float> pressure,
                        std::optional<float> voltage,
                        std::optional<int> rpm, std::optional<float> torque)
    {
        ModelDDeltaSignal signal;
        signal.numericIndex = index_;
        signal.temperature  = temperature;
        signal.pressure     = pressure;
        signal.voltage      = voltage;
        signal.rpm          = rpm;
        signal.torque       = torque;
        dispatcher_.broadcast(alias_, signal);
    }

private:
    void onCommand(const ModelDCommandSignal& signal) {
        std::cout << "[ModelD " << index_ << "] command received:"
                  << " temperature=" << signal.temperature << "\n";
    }

    sd::SignalDispatcher& dispatcher_;
    int         index_;
    std::string alias_;
};

} // namespace dc
