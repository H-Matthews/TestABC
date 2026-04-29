#pragma once

#include "SignalDispatcher.h"
#include "DeltaSignal.h"
#include "ModelCState.h"
#include "ModelCCommandSignal.h"

#include <iostream>
#include <optional>
#include <string>

namespace dc {

class ModelCComponent {
public:
    ModelCComponent(sd::SignalDispatcher& dispatcher, int index)
        : dispatcher_(dispatcher)
        , index_(index)
        , alias_("model_c." + std::to_string(index))
    {
        dispatcher_.bind<ModelCCommandSignal>(alias_, alias_, this, &ModelCComponent::onCommand);
    }

    ~ModelCComponent() {
        dispatcher_.disconnect(alias_);
    }

    void broadcastDelta(std::optional<float> temperature, std::optional<float> pressure,
                        std::optional<int> rpm, std::optional<float> torque)
    {
        DeltaSignal<ModelCState> signal;
        signal.numericIndex = index_;
        signal.temperature  = temperature;
        signal.pressure     = pressure;
        signal.rpm          = rpm;
        signal.torque       = torque;
        dispatcher_.broadcast(alias_, signal);
    }

private:
    void onCommand(const ModelCCommandSignal& signal) {
        std::cout << "[ModelC " << index_ << "] command received:"
                  << " temperature=" << signal.temperature << "\n";
    }

    sd::SignalDispatcher& dispatcher_;
    int         index_;
    std::string alias_;
};

} // namespace dc
