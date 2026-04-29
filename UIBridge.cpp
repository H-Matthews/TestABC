#include "UIBridge.h"
#include "ModelACommandSignal.h"
#include "ModelBCommandSignal.h"
#include "ModelCCommandSignal.h"
#include "ModelDCommandSignal.h"
#include "ModelECommandSignal.h"
#include "ModelFCommandSignal.h"
#include "ModelGCommandSignal.h"
#include "ModelHCommandSignal.h"
#include "ModelICommandSignal.h"

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
    dataCache_.setCallback<ModelCState>(
        [this](int index, std::shared_ptr<const ModelCState> snap) {
            this->onModelCReady(index, std::move(snap));
        });
    dataCache_.setCallback<ModelDState>(
        [this](int index, std::shared_ptr<const ModelDState> snap) {
            this->onModelDReady(index, std::move(snap));
        });
    dataCache_.setCallback<ModelEState>(
        [this](int index, std::shared_ptr<const ModelEState> snap) {
            this->onModelEReady(index, std::move(snap));
        });
    dataCache_.setCallback<ModelFState>(
        [this](int index, std::shared_ptr<const ModelFState> snap) {
            this->onModelFReady(index, std::move(snap));
        });
    dataCache_.setCallback<ModelGState>(
        [this](int index, std::shared_ptr<const ModelGState> snap) {
            this->onModelGReady(index, std::move(snap));
        });
    dataCache_.setCallback<ModelHState>(
        [this](int index, std::shared_ptr<const ModelHState> snap) {
            this->onModelHReady(index, std::move(snap));
        });
    dataCache_.setCallback<ModelIState>(
        [this](int index, std::shared_ptr<const ModelIState> snap) {
            this->onModelIReady(index, std::move(snap));
        });

    dataCache_.registerHandlers(dispatcher_);
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

void UIBridge::sendModelCCommand(int index, float temperature) {
    ModelCCommandSignal signal;
    signal.numericIndex = index;
    signal.temperature  = temperature;
    dispatcher_.sendTo("model_c." + std::to_string(index), signal);
}

void UIBridge::sendModelDCommand(int index, float temperature) {
    ModelDCommandSignal signal;
    signal.numericIndex = index;
    signal.temperature  = temperature;
    dispatcher_.sendTo("model_d." + std::to_string(index), signal);
}

void UIBridge::sendModelECommand(int index, float voltage) {
    ModelECommandSignal signal;
    signal.numericIndex = index;
    signal.voltage      = voltage;
    dispatcher_.sendTo("model_e." + std::to_string(index), signal);
}

void UIBridge::sendModelFCommand(int index, float temperature) {
    ModelFCommandSignal signal;
    signal.numericIndex = index;
    signal.temperature  = temperature;
    dispatcher_.sendTo("model_f." + std::to_string(index), signal);
}

void UIBridge::sendModelGCommand(int index, float position) {
    ModelGCommandSignal signal;
    signal.numericIndex = index;
    signal.position     = position;
    dispatcher_.sendTo("model_g." + std::to_string(index), signal);
}

void UIBridge::sendModelHCommand(int index, int rpm) {
    ModelHCommandSignal signal;
    signal.numericIndex = index;
    signal.rpm          = rpm;
    dispatcher_.sendTo("model_h." + std::to_string(index), signal);
}

void UIBridge::sendModelICommand(int index, float temperature) {
    ModelICommandSignal signal;
    signal.numericIndex = index;
    signal.temperature  = temperature;
    dispatcher_.sendTo("model_i." + std::to_string(index), signal);
}

void UIBridge::onModelAReady(int index, std::shared_ptr<const ModelAState> snapshot) {
    std::cout << "[UIBridge] ModelA[" << index << "] snapshot ready:"
              << " temperature=" << snapshot->temperature.value()
              << " pressure="    << snapshot->pressure.value()
              << " voltage="     << snapshot->voltage.value()
              << " flowRate="    << snapshot->flowRate.value()
              << "\n";
}

void UIBridge::onModelBReady(int index, std::shared_ptr<const ModelBState> snapshot) {
    std::cout << "[UIBridge] ModelB[" << index << "] snapshot ready:"
              << " rpm="    << snapshot->rpm.value()
              << " torque=" << snapshot->torque.value()
              << "\n";
}

void UIBridge::onModelCReady(int index, std::shared_ptr<const ModelCState> snapshot) {
    std::cout << "[UIBridge] ModelC[" << index << "] snapshot ready:"
              << " temperature=" << snapshot->temperature.value()
              << " pressure="    << snapshot->pressure.value()
              << " rpm="         << snapshot->rpm.value()
              << " torque="      << snapshot->torque.value()
              << "\n";
}

void UIBridge::onModelDReady(int index, std::shared_ptr<const ModelDState> snapshot) {
    std::cout << "[UIBridge] ModelD[" << index << "] snapshot ready:"
              << " temperature=" << snapshot->temperature.value()
              << " pressure="    << snapshot->pressure.value()
              << " voltage="     << snapshot->voltage.value()
              << " rpm="         << snapshot->rpm.value()
              << " torque="      << snapshot->torque.value()
              << "\n";
}

void UIBridge::onModelEReady(int index, std::shared_ptr<const ModelEState> snapshot) {
    std::cout << "[UIBridge] ModelE[" << index << "] snapshot ready:"
              << " voltage="   << snapshot->voltage.value()
              << " current="   << snapshot->current.value()
              << " frequency=" << snapshot->frequency.value()
              << "\n";
}

void UIBridge::onModelFReady(int index, std::shared_ptr<const ModelFState> snapshot) {
    std::cout << "[UIBridge] ModelF[" << index << "] snapshot ready:"
              << " temperature=" << snapshot->temperature.value()
              << " pressure="    << snapshot->pressure.value()
              << " humidity="    << snapshot->humidity.value()
              << " flowRate="    << snapshot->flowRate.value()
              << "\n";
}

void UIBridge::onModelGReady(int index, std::shared_ptr<const ModelGState> snapshot) {
    std::cout << "[UIBridge] ModelG[" << index << "] snapshot ready:"
              << " rpm="      << snapshot->rpm.value()
              << " torque="   << snapshot->torque.value()
              << " position=" << snapshot->position.value()
              << " velocity=" << snapshot->velocity.value()
              << "\n";
}

void UIBridge::onModelHReady(int index, std::shared_ptr<const ModelHState> snapshot) {
    std::cout << "[UIBridge] ModelH[" << index << "] snapshot ready:"
              << " voltage=" << snapshot->voltage.value()
              << " rpm="     << snapshot->rpm.value()
              << " torque="  << snapshot->torque.value()
              << "\n";
}

void UIBridge::onModelIReady(int index, std::shared_ptr<const ModelIState> snapshot) {
    std::cout << "[UIBridge] ModelI[" << index << "] snapshot ready:"
              << " temperature=" << snapshot->temperature.value()
              << " pressure="    << snapshot->pressure.value()
              << " density="     << snapshot->density.value()
              << " viscosity="   << snapshot->viscosity.value()
              << "\n";
}

} // namespace dc
