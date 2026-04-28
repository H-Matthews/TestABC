#pragma once

#include "SignalDispatcher.h"
#include "ModelADeltaSignal.h"
#include "ModelACommandSignal.h"

#include <iostream>
#include <string>

namespace dc {

// Minimal model component that can broadcast delta signals and receive commands.
class ModelAComponent {
public:
    ModelAComponent(sd::SignalDispatcher& dispatcher, int index)
        : dispatcher_(dispatcher)
        , index_(index)
        , alias_("model_a." + std::to_string(index))
    {
        dispatcher_.bind<ModelACommandSignal>(
            alias_, alias_, this, &ModelAComponent::onCommand);
    }

    ~ModelAComponent() {
        dispatcher_.disconnect(alias_);
    }

    void broadcastDelta(float temperature, float pressure,
                        float flowRate,    float voltage,
                        uint32_t presentFields)
    {
        ModelADeltaSignal signal;
        signal.numericIndex  = index_;
        signal.presentFields = presentFields;
        signal.temperature   = temperature;
        signal.pressure      = pressure;
        signal.flowRate      = flowRate;
        signal.voltage       = voltage;
        dispatcher_.broadcast(alias_, signal);
    }

private:
    void onCommand(const ModelACommandSignal& signal) {
        std::cout << "[ModelA " << index_ << "] command received:"
                  << " temperature=" << signal.temperature << "\n";
    }

    sd::SignalDispatcher& dispatcher_;
    int         index_;
    std::string alias_;
};

} // NAMESPACE DCs