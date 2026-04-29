#include "DataCache.h"

namespace dc {

DataCache::~DataCache() {
    if (dispatcher_) {
        dispatcher_->disconnect("data-cache");
    }
}

void DataCache::registerHandlers(sd::SignalDispatcher& dispatcher) {
    dispatcher_ = &dispatcher;
    dispatcher.bind<DeltaSignal<ModelAState>>("data-cache", "data-cache", this, &DataCache::onModelADelta);
    dispatcher.bind<DeltaSignal<ModelBState>>("data-cache", "data-cache", this, &DataCache::onModelBDelta);
    dispatcher.bind<DeltaSignal<ModelCState>>("data-cache", "data-cache", this, &DataCache::onModelCDelta);
    dispatcher.bind<DeltaSignal<ModelDState>>("data-cache", "data-cache", this, &DataCache::onModelDDelta);
    dispatcher.bind<DeltaSignal<ModelEState>>("data-cache", "data-cache", this, &DataCache::onModelEDelta);
    dispatcher.bind<DeltaSignal<ModelFState>>("data-cache", "data-cache", this, &DataCache::onModelFDelta);
    dispatcher.bind<DeltaSignal<ModelGState>>("data-cache", "data-cache", this, &DataCache::onModelGDelta);
    dispatcher.bind<DeltaSignal<ModelHState>>("data-cache", "data-cache", this, &DataCache::onModelHDelta);
    dispatcher.bind<DeltaSignal<ModelIState>>("data-cache", "data-cache", this, &DataCache::onModelIDelta);
}

} // namespace dc
