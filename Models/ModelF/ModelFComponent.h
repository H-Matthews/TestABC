#pragma once

#include "SignalDispatcher.h"
#include "ModelFDeltaSignal.h"
#include "ModelFCommandSignal.h"

#include <iostream>
#include <optional>
#include <string>

namespace dc {

class ModelFComponent {
public:
    ModelFComponent(sd::SignalDispatcher& dispatcher, int index)
        : dispatcher_(dispatcher)
        , index_(index)
        , alias_("model_f." + std::to_string(index))
    {
        dispatcher_.bind<ModelFCommandSignal>(alias_, alias_, this, &ModelFComponent::onCommand);
    }

    ~ModelFComponent() {
        dispatcher_.disconnect(alias_);
    }

    void broadcastDelta(std::optional<float> temperature, std::optional<float> pressure,
                        std::optional<float> humidity,    std::optional<float> flowRate)
    {
        ModelFDeltaSignal signal;
        signal.numericIndex = index_;
        signal.temperature  = temperature;
        signal.pressure     = pressure;
        signal.humidity     = humidity;
        signal.flowRate     = flowRate;
        dispatcher_.broadcast(alias_, signal);
    }

private:
    void onCommand(const ModelFCommandSignal& signal) {
        std::cout << "[ModelF " << index_ << "] command received:"
                  << " temperature=" << signal.temperature << "\n";
    }

    sd::SignalDispatcher& dispatcher_;
    int         index_;
    std::string alias_;
};

} // namespace dc
