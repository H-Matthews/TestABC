#include "UIBridge.h"
#include "Signals.h"
#include "SignalDispatcher.h"

#include <iostream>
#include <string>

namespace dc {

// Minimal model component that can broadcast delta signals and receive commands.
class ModelAComponent {
public:
    ModelAComponent(sd::SignalDispatcher& dispatcher, int index)
        : dispatcher_(dispatcher)
        , index_(index)
        , alias_("model_a." + std::to_string(index))
    {
        dispatcher_.bind<ModelACommandSignal>(
            alias_, alias_, this, &ModelAComponent::onCommand);
    }

    ~ModelAComponent() {
        dispatcher_.disconnect(alias_);
    }

    void broadcastDelta(float temperature, float pressure,
                        float flowRate,    float voltage,
                        uint32_t presentFields)
    {
        ModelADeltaSignal signal;
        signal.numericIndex  = index_;
        signal.presentFields = presentFields;
        signal.temperature   = temperature;
        signal.pressure      = pressure;
        signal.flowRate      = flowRate;
        signal.voltage       = voltage;
        dispatcher_.broadcast(alias_, signal);
    }

private:
    void onCommand(const ModelACommandSignal& signal) {
        std::cout << "[ModelA " << index_ << "] command received:"
                  << " temperature=" << signal.temperature << "\n";
    }

    sd::SignalDispatcher& dispatcher_;
    int         index_;
    std::string alias_;
};

class ModelBComponent {
public:
    ModelBComponent(sd::SignalDispatcher& dispatcher, int index)
        : dispatcher_(dispatcher)
        , index_(index)
        , alias_("model_b." + std::to_string(index))
    {
        dispatcher_.bind<ModelBCommandSignal>(
            alias_, alias_, this, &ModelBComponent::onCommand);
    }

    ~ModelBComponent() {
        dispatcher_.disconnect(alias_);
    }

    void broadcastDelta(int rpm, float torque, uint32_t presentFields) {
        ModelBDeltaSignal signal;
        signal.numericIndex  = index_;
        signal.presentFields = presentFields;
        signal.rpm           = rpm;
        signal.torque        = torque;
        dispatcher_.broadcast(alias_, signal);
    }

private:
    void onCommand(const ModelBCommandSignal& signal) {
        std::cout << "[ModelB " << index_ << "] command received:"
                  << " rpm=" << signal.rpm << "\n";
    }

    sd::SignalDispatcher& dispatcher_;
    int         index_;
    std::string alias_;
};

} // namespace dc

int main() {
    sd::SignalDispatcher dispatcher;

    // UIBridge construction wires DataCache callbacks and registers delta handlers.
    dc::UIBridge uiBridge(dispatcher);

    // Model instances register their command handlers in constructors.
    dc::ModelAComponent modelA0(dispatcher, 0);
    dc::ModelAComponent modelA1(dispatcher, 1);
    dc::ModelBComponent modelB0(dispatcher, 0);

    // Pre-simulation: reserve instance records per model type.
    uiBridge.reserveInstances<dc::ModelAState>(2);
    uiBridge.reserveInstances<dc::ModelBState>(1);

    std::cout << "--- Push flow: ModelA instance 0 ---\n";
    // Component 1 broadcasts Temperature + Pressure. Not ready yet.
    modelA0.broadcastDelta(
        25.0f, 1.013f, 0.0f, 0.0f,
        static_cast<uint32_t>(dc::ModelAField::Temperature) |
        static_cast<uint32_t>(dc::ModelAField::Pressure));
    std::cout << "(no callback yet — FlowRate and Voltage still missing)\n";

    // Component 2 broadcasts FlowRate + Voltage. Now all four fields seen → callback fires.
    modelA0.broadcastDelta(
        0.0f, 0.0f, 5.5f, 12.0f,
        static_cast<uint32_t>(dc::ModelAField::FlowRate) |
        static_cast<uint32_t>(dc::ModelAField::Voltage));

    std::cout << "\n--- Push flow: ModelA instance 1 (all fields in one burst) ---\n";
    modelA1.broadcastDelta(
        30.0f, 1.1f, 6.0f, 11.5f,
        dc::kModelAAllFields);

    std::cout << "\n--- Push flow: ModelB instance 0 ---\n";
    modelB0.broadcastDelta(
        1500, 45.0f,
        static_cast<uint32_t>(dc::ModelBField::Rpm) |
        static_cast<uint32_t>(dc::ModelBField::Torque));

    std::cout << "\n--- Query flow: on-demand query for ModelA instance 0 ---\n";
    auto result = uiBridge.queryModelA(0);
    if (result.status == dc::QueryResult<dc::ModelAState>::Status::Ready) {
        std::cout << "[main] Query ready: temperature=" << result.snapshot->temperature
                  << " pressure=" << result.snapshot->pressure << "\n";
    }

    std::cout << "\n--- Query flow: on-demand query for unknown index ---\n";
    auto badResult = uiBridge.queryModelA(99);
    if (badResult.status == dc::QueryResult<dc::ModelAState>::Status::UnknownIndex) {
        std::cout << "[main] Correctly reported UnknownIndex for index 99\n";
    }

    std::cout << "\n--- Command flow: UIBridge sends command to ModelA instance 0 ---\n";
    uiBridge.sendModelACommand(0, 35.0f);

    std::cout << "\n--- Command flow: UIBridge sends command to ModelB instance 0 ---\n";
    uiBridge.sendModelBCommand(0, 2000);

    return 0;
}
