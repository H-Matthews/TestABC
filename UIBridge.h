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

    // On-demand queries — bypass the dispatcher.
    QueryResult<ModelAState> queryModelA(int index) const;
    QueryResult<ModelBState> queryModelB(int index) const;

    // Send a command to a specific ModelA instance via the dispatcher.
    void sendModelACommand(int index, float temperature);
    void sendModelBCommand(int index, int rpm);

private:
    void onModelAReady(int index, std::shared_ptr<const ModelAState> snapshot);
    void onModelBReady(int index, std::shared_ptr<const ModelBState> snapshot);

    sd::SignalDispatcher& dispatcher_;
    DataCache dataCache_;
};

} // namespace dc
