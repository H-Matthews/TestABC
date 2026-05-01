#pragma once

#include "SignalDispatcher.h"
#include "DeltaSignal.h"
#include "ModelEState.h"
#include "ModelECommandSignal.h"

#include <iostream>
#include <optional>
#include <string>

namespace dc {

class ModelEComponent {
public:
    ModelEComponent(sd::SignalDispatcher& dispatcher, int index)
        : dispatcher_(dispatcher)
        , index_(index)
        , alias_("model_e." + std::to_string(index))
    {
        dispatcher_.bind<ModelECommandSignal>(alias_, alias_, [this](const ModelECommandSignal& s) { onCommand(s); });
    }

    ~ModelEComponent() {
        dispatcher_.disconnect(alias_);
    }

    void broadcastDelta(std::optional<float> voltage,
                        std::optional<float> current,
                        std::optional<float> frequency)
    {
        DeltaSignal<ModelEState> signal;
        signal.numericIndex = index_;
        signal.voltage      = voltage;
        signal.current      = current;
        signal.frequency    = frequency;
        dispatcher_.broadcast(alias_, signal);
    }

private:
    void onCommand(const ModelECommandSignal& signal) {
        std::cout << "[ModelE " << index_ << "] command received:"
                  << " voltage=" << signal.voltage << "\n";
    }

    sd::SignalDispatcher& dispatcher_;
    int         index_;
    std::string alias_;
};

} // namespace dc
