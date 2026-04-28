#include "UIBridge.h"
#include "ModelACommandSignal.h"
#include "ModelBCommandSignal.h"

#include <iostream>
#include <string>

namespace dc {

UIBridge::UIBridge(sd::SignalDispatcher& dispatcher)
    : dispatcher_(dispatcher)
{
    dataCache_.setCallback<ModelAState>(
        [this](int index, std::shared_ptr<const ModelAState> snap) {
            this->onModelAReady(index, std::move(snap));
        });

    dataCache_.setCallback<ModelBState>(
        [this](int index, std::shared_ptr<const ModelBState> snap) {
            this->onModelBReady(index, std::move(snap));
        });

    dataCache_.registerHandlers(dispatcher_);
}


QueryResult<ModelAState> UIBridge::queryModelA(int index) const {
    return dataCache_.query<ModelAState>(index);
}

QueryResult<ModelBState> UIBridge::queryModelB(int index) const {
    return dataCache_.query<ModelBState>(index);
}

void UIBridge::sendModelACommand(int index, float temperature) {
    ModelACommandSignal signal;
    signal.numericIndex = index;
    signal.temperature  = temperature;
    dispatcher_.sendTo("model_a." + std::to_string(index), signal);
}

void UIBridge::sendModelBCommand(int index, int rpm) {
    ModelBCommandSignal signal;
    signal.numericIndex = index;
    signal.rpm          = rpm;
    dispatcher_.sendTo("model_b." + std::to_string(index), signal);
}

void UIBridge::onModelAReady(int index, std::shared_ptr<const ModelAState> snapshot) {
    std::cout << "[UIBridge] ModelA[" << index << "] snapshot ready:"
              << " temperature=" << snapshot->temperature.value()
              << " pressure="    << snapshot->pressure.value()
              << " flowRate="    << snapshot->flowRate.value()
              << " voltage="     << snapshot->voltage.value()
              << "\n";
}

void UIBridge::onModelBReady(int index, std::shared_ptr<const ModelBState> snapshot) {
    std::cout << "[UIBridge] ModelB[" << index << "] snapshot ready:"
              << " rpm="    << snapshot->rpm.value()
              << " torque=" << snapshot->torque.value()
              << "\n";
}

} // namespace dc
