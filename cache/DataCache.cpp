#include "DataCache.h"

namespace dc {

DataCache::~DataCache() {
    if (dispatcher_) {
        dispatcher_->disconnect("data-cache");
    }
}

void DataCache::mergeModelA(ModelAState& state, const ModelADeltaSignal& delta) {
    if (delta.temperature) { state.temperature = delta.temperature; }
    if (delta.pressure)    { state.pressure    = delta.pressure;    }
    if (delta.flowRate)    { state.flowRate     = delta.flowRate;    }
    if (delta.voltage)     { state.voltage      = delta.voltage;     }
}

void DataCache::mergeModelB(ModelBState& state, const ModelBDeltaSignal& delta) {
    if (delta.rpm)    { state.rpm    = delta.rpm;    }
    if (delta.torque) { state.torque = delta.torque; }
}

void DataCache::onModelADelta(const ModelADeltaSignal& signal) {
    onDelta<ModelAState>(signal, mergeModelA);
}

void DataCache::onModelBDelta(const ModelBDeltaSignal& signal) {
    onDelta<ModelBState>(signal, mergeModelB);
}

void DataCache::registerHandlers(sd::SignalDispatcher& dispatcher) {
    dispatcher_ = &dispatcher;
    dispatcher.bind<ModelADeltaSignal>("data-cache", "data-cache", this, &DataCache::onModelADelta);
    dispatcher.bind<ModelBDeltaSignal>("data-cache", "data-cache", this, &DataCache::onModelBDelta);
}

} // namespace dc
