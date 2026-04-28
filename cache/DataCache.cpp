#include "DataCache.h"

#include <iostream>
#include <shared_mutex>

namespace dc {

DataCache::~DataCache() {
    if (dispatcher_) {
        dispatcher_->disconnect("data-cache");
    }
}

void DataCache::registerHandlers(sd::SignalDispatcher& dispatcher) {
    dispatcher_ = &dispatcher;
    dispatcher.bind<ModelADeltaSignal>(
        "data-cache", "data-cache", this, &DataCache::onModelADelta);
    dispatcher.bind<ModelBDeltaSignal>(
        "data-cache", "data-cache", this, &DataCache::onModelBDelta);
}

void DataCache::onModelADelta(const ModelADeltaSignal& signal) {
    InstanceRecord<ModelAState>* record = nullptr;
    {
        std::shared_lock lock(modelAMutex_);
        auto it = modelARecords_.find(signal.numericIndex);
        if (it == modelARecords_.end()) {
            std::cerr << "[DataCache] Warning: ModelA delta for unknown index "
                      << signal.numericIndex << "\n";
            return;
        }
        record = it->second.get();
    }

    // Lock ordering: map mutex released above before instance mutex acquired inside applyDelta.
    bool ready = record->applyDelta(signal, kModelAAllFields,
        [](ModelAState& state, const ModelADeltaSignal& delta) {
            if (delta.presentFields & static_cast<uint32_t>(ModelAField::Temperature))
                state.temperature = delta.temperature;
            if (delta.presentFields & static_cast<uint32_t>(ModelAField::Pressure))
                state.pressure = delta.pressure;
            if (delta.presentFields & static_cast<uint32_t>(ModelAField::FlowRate))
                state.flowRate = delta.flowRate;
            if (delta.presentFields & static_cast<uint32_t>(ModelAField::Voltage))
                state.voltage = delta.voltage;
        });

    if (ready) {
        publish<ModelAState>(signal.numericIndex, record->snapshot());
    }
}

void DataCache::onModelBDelta(const ModelBDeltaSignal& signal) {
    InstanceRecord<ModelBState>* record = nullptr;
    {
        std::shared_lock lock(modelBMutex_);
        auto it = modelBRecords_.find(signal.numericIndex);
        if (it == modelBRecords_.end()) {
            std::cerr << "[DataCache] Warning: ModelB delta for unknown index "
                      << signal.numericIndex << "\n";
            return;
        }
        record = it->second.get();
    }

    bool ready = record->applyDelta(signal, kModelBAllFields,
        [](ModelBState& state, const ModelBDeltaSignal& delta) {
            if (delta.presentFields & static_cast<uint32_t>(ModelBField::Rpm))
                state.rpm = delta.rpm;
            if (delta.presentFields & static_cast<uint32_t>(ModelBField::Torque))
                state.torque = delta.torque;
        });

    if (ready) {
        publish<ModelBState>(signal.numericIndex, record->snapshot());
    }
}

} // namespace dc
