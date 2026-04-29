#pragma once

#include "SignalDispatcher.h"
#include "ModelIDeltaSignal.h"
#include "ModelICommandSignal.h"

#include <iostream>
#include <optional>
#include <string>

namespace dc {

class ModelIComponent {
public:
    ModelIComponent(sd::SignalDispatcher& dispatcher, int index)
        : dispatcher_(dispatcher)
        , index_(index)
        , alias_("model_i." + std::to_string(index))
    {
        dispatcher_.bind<ModelICommandSignal>(alias_, alias_, this, &ModelIComponent::onCommand);
    }

    ~ModelIComponent() {
        dispatcher_.disconnect(alias_);
    }

    void broadcastDelta(std::optional<float> temperature, std::optional<float> pressure,
                        std::optional<float> density,     std::optional<float> viscosity)
    {
        ModelIDeltaSignal signal;
        signal.numericIndex = index_;
        signal.temperature  = temperature;
        signal.pressure     = pressure;
        signal.density      = density;
        signal.viscosity    = viscosity;
        dispatcher_.broadcast(alias_, signal);
    }

private:
    void onCommand(const ModelICommandSignal& signal) {
        std::cout << "[ModelI " << index_ << "] command received:"
                  << " temperature=" << signal.temperature << "\n";
    }

    sd::SignalDispatcher& dispatcher_;
    int         index_;
    std::string alias_;
};

} // namespace dc
