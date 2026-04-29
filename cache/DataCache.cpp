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

void DataCache::mergeModelC(ModelCState& state, const ModelCDeltaSignal& delta) {
    if (delta.temperature) { state.temperature = delta.temperature; }
    if (delta.pressure)    { state.pressure    = delta.pressure;    }
    if (delta.rpm)         { state.rpm         = delta.rpm;         }
    if (delta.torque)      { state.torque      = delta.torque;      }
}

void DataCache::mergeModelD(ModelDState& state, const ModelDDeltaSignal& delta) {
    if (delta.temperature) { state.temperature = delta.temperature; }
    if (delta.pressure)    { state.pressure    = delta.pressure;    }
    if (delta.voltage)     { state.voltage      = delta.voltage;     }
    if (delta.rpm)         { state.rpm         = delta.rpm;         }
    if (delta.torque)      { state.torque      = delta.torque;      }
}

void DataCache::mergeModelE(ModelEState& state, const ModelEDeltaSignal& delta) {
    if (delta.voltage)   { state.voltage    = delta.voltage;   }
    if (delta.current)   { state.current    = delta.current;   }
    if (delta.frequency) { state.frequency  = delta.frequency; }
}

void DataCache::mergeModelF(ModelFState& state, const ModelFDeltaSignal& delta) {
    if (delta.temperature) { state.temperature = delta.temperature; }
    if (delta.pressure)    { state.pressure    = delta.pressure;    }
    if (delta.humidity)    { state.humidity    = delta.humidity;    }
    if (delta.flowRate)    { state.flowRate     = delta.flowRate;    }
}

void DataCache::mergeModelG(ModelGState& state, const ModelGDeltaSignal& delta) {
    if (delta.rpm)      { state.rpm      = delta.rpm;      }
    if (delta.torque)   { state.torque   = delta.torque;   }
    if (delta.position) { state.position = delta.position; }
    if (delta.velocity) { state.velocity = delta.velocity; }
}

void DataCache::mergeModelH(ModelHState& state, const ModelHDeltaSignal& delta) {
    if (delta.voltage) { state.voltage = delta.voltage; }
    if (delta.rpm)     { state.rpm     = delta.rpm;     }
    if (delta.torque)  { state.torque  = delta.torque;  }
}

void DataCache::mergeModelI(ModelIState& state, const ModelIDeltaSignal& delta) {
    if (delta.temperature) { state.temperature = delta.temperature; }
    if (delta.pressure)    { state.pressure    = delta.pressure;    }
    if (delta.density)     { state.density     = delta.density;     }
    if (delta.viscosity)   { state.viscosity   = delta.viscosity;   }
}

void DataCache::onModelADelta(const ModelADeltaSignal& signal) {
    onDelta<ModelAState>(signal, mergeModelA);
}

void DataCache::onModelBDelta(const ModelBDeltaSignal& signal) {
    onDelta<ModelBState>(signal, mergeModelB);
}

void DataCache::onModelCDelta(const ModelCDeltaSignal& signal) {
    onDelta<ModelCState>(signal, mergeModelC);
}

void DataCache::onModelDDelta(const ModelDDeltaSignal& signal) {
    onDelta<ModelDState>(signal, mergeModelD);
}

void DataCache::onModelEDelta(const ModelEDeltaSignal& signal) {
    onDelta<ModelEState>(signal, mergeModelE);
}

void DataCache::onModelFDelta(const ModelFDeltaSignal& signal) {
    onDelta<ModelFState>(signal, mergeModelF);
}

void DataCache::onModelGDelta(const ModelGDeltaSignal& signal) {
    onDelta<ModelGState>(signal, mergeModelG);
}

void DataCache::onModelHDelta(const ModelHDeltaSignal& signal) {
    onDelta<ModelHState>(signal, mergeModelH);
}

void DataCache::onModelIDelta(const ModelIDeltaSignal& signal) {
    onDelta<ModelIState>(signal, mergeModelI);
}

void DataCache::registerHandlers(sd::SignalDispatcher& dispatcher) {
    dispatcher_ = &dispatcher;
    dispatcher.bind<ModelADeltaSignal>("data-cache", "data-cache", this, &DataCache::onModelADelta);
    dispatcher.bind<ModelBDeltaSignal>("data-cache", "data-cache", this, &DataCache::onModelBDelta);
    dispatcher.bind<ModelCDeltaSignal>("data-cache", "data-cache", this, &DataCache::onModelCDelta);
    dispatcher.bind<ModelDDeltaSignal>("data-cache", "data-cache", this, &DataCache::onModelDDelta);
    dispatcher.bind<ModelEDeltaSignal>("data-cache", "data-cache", this, &DataCache::onModelEDelta);
    dispatcher.bind<ModelFDeltaSignal>("data-cache", "data-cache", this, &DataCache::onModelFDelta);
    dispatcher.bind<ModelGDeltaSignal>("data-cache", "data-cache", this, &DataCache::onModelGDelta);
    dispatcher.bind<ModelHDeltaSignal>("data-cache", "data-cache", this, &DataCache::onModelHDelta);
    dispatcher.bind<ModelIDeltaSignal>("data-cache", "data-cache", this, &DataCache::onModelIDelta);
}

} // namespace dc
