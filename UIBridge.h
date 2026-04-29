#pragma once

#include "DataCache.h"
#include "QueryResult.h"
#include "SignalDispatcher.h"

#include <memory>

namespace dc {

class UIBridge {
public:
    explicit UIBridge(sd::SignalDispatcher& dispatcher);
    ~UIBridge() = default;

    UIBridge(const UIBridge&)            = delete;
    UIBridge& operator=(const UIBridge&) = delete;

    // Pre-allocate instance records for a model type. Call once per type before simulation begins.
    template<typename TState>
    void reserveInstances(int count) {
        dataCache_.reserve<TState>(count);
    }

    // Dynamically register a single instance at runtime. Safe to call after simulation has started.
    template<typename TState>
    void addInstance(int index) {
        dataCache_.addInstance<TState>(index);
    }

    // On-demand query — bypasses the dispatcher.
    template<typename TState>
    QueryResult<TState> query(int index) const {
        return dataCache_.query<TState>(index);
    }

    // Send a command to a specific model instance via the dispatcher.
    void sendModelACommand(int index, float temperature);
    void sendModelBCommand(int index, int rpm);
    void sendModelCCommand(int index, float temperature);
    void sendModelDCommand(int index, float temperature);
    void sendModelECommand(int index, float voltage);
    void sendModelFCommand(int index, float temperature);
    void sendModelGCommand(int index, float position);
    void sendModelHCommand(int index, int rpm);
    void sendModelICommand(int index, float temperature);

private:
    void onModelAReady(int index, std::shared_ptr<const ModelAState> snapshot);
    void onModelBReady(int index, std::shared_ptr<const ModelBState> snapshot);
    void onModelCReady(int index, std::shared_ptr<const ModelCState> snapshot);
    void onModelDReady(int index, std::shared_ptr<const ModelDState> snapshot);
    void onModelEReady(int index, std::shared_ptr<const ModelEState> snapshot);
    void onModelFReady(int index, std::shared_ptr<const ModelFState> snapshot);
    void onModelGReady(int index, std::shared_ptr<const ModelGState> snapshot);
    void onModelHReady(int index, std::shared_ptr<const ModelHState> snapshot);
    void onModelIReady(int index, std::shared_ptr<const ModelIState> snapshot);

    sd::SignalDispatcher& dispatcher_;
    DataCache dataCache_;
};

} // namespace dc
