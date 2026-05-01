#include "DataCache.h"

namespace dc {

DataCache::~DataCache() {
    if (dispatcher_) {
        dispatcher_->disconnect("data-cache");
    }
}

void DataCache::registerHandlers(sd::SignalDispatcher& dispatcher) {
    dispatcher_ = &dispatcher;
    dispatcher.bind<DeltaSignal<ModelAState>>("data-cache", "data-cache", [this](const DeltaSignal<ModelAState>& s) { onDelta<ModelAState>(s); });
    dispatcher.bind<DeltaSignal<ModelBState>>("data-cache", "data-cache", [this](const DeltaSignal<ModelBState>& s) { onDelta<ModelBState>(s); });
    dispatcher.bind<DeltaSignal<ModelCState>>("data-cache", "data-cache", [this](const DeltaSignal<ModelCState>& s) { onDelta<ModelCState>(s); });
    dispatcher.bind<DeltaSignal<ModelDState>>("data-cache", "data-cache", [this](const DeltaSignal<ModelDState>& s) { onDelta<ModelDState>(s); });
    dispatcher.bind<DeltaSignal<ModelEState>>("data-cache", "data-cache", [this](const DeltaSignal<ModelEState>& s) { onDelta<ModelEState>(s); });
    dispatcher.bind<DeltaSignal<ModelFState>>("data-cache", "data-cache", [this](const DeltaSignal<ModelFState>& s) { onDelta<ModelFState>(s); });
    dispatcher.bind<DeltaSignal<ModelGState>>("data-cache", "data-cache", [this](const DeltaSignal<ModelGState>& s) { onDelta<ModelGState>(s); });
    dispatcher.bind<DeltaSignal<ModelHState>>("data-cache", "data-cache", [this](const DeltaSignal<ModelHState>& s) { onDelta<ModelHState>(s); });
    dispatcher.bind<DeltaSignal<ModelIState>>("data-cache", "data-cache", [this](const DeltaSignal<ModelIState>& s) { onDelta<ModelIState>(s); });
}

} // namespace dc
