#include "UIBridge.h"
#include "ModelAComponent.h"
#include "ModelBComponent.h"
#include "SignalDispatcher.h"

#include <iostream>
#include <string>

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
    modelA0.broadcastDelta(25.0f, 1.013f, std::nullopt, std::nullopt);
    std::cout << "(no callback yet — FlowRate and Voltage still missing)\n";

    // Component 2 broadcasts FlowRate + Voltage. Now all four fields seen → callback fires.
    modelA0.broadcastDelta(std::nullopt, std::nullopt, 5.5f, 12.0f);

    std::cout << "\n--- Push flow: ModelA instance 1 (all fields in one burst) ---\n";
    modelA1.broadcastDelta(30.0f, 1.1f, 6.0f, 11.5f);

    std::cout << "\n--- Push flow: ModelB instance 0 ---\n";
    modelB0.broadcastDelta(1500, 45.0f);

    std::cout << "\n--- Query flow: on-demand query for ModelA instance 0 ---\n";
    auto result = uiBridge.queryModelA(0);
    if (result.status == dc::QueryResult<dc::ModelAState>::Status::Ready) {
        std::cout << "[main] Query ready: temperature=" << result.snapshot->temperature.value()
                  << " pressure=" << result.snapshot->pressure.value() << "\n";
    }

    std::cout << "\n--- Query flow: on-demand query for unknown index ---\n";
    auto badResult = uiBridge.queryModelA(99);
    if (badResult.status == dc::QueryResult<dc::ModelAState>::Status::UnknownIndex) {
        std::cout << "[main] Correctly reported UnknownIndex for index 99\n";
    }

    std::cout << "\n--- Dynamic add: ModelA instance 2 comes online during simulation ---\n";
    uiBridge.addInstance<dc::ModelAState>(2);
    dc::ModelAComponent modelA2(dispatcher, 2);
    modelA2.broadcastDelta(22.0f, 0.9f, 4.0f, 10.0f);

    std::cout << "\n--- Command flow: UIBridge sends command to ModelA instance 0 ---\n";
    uiBridge.sendModelACommand(0, 35.0f);

    std::cout << "\n--- Command flow: UIBridge sends command to ModelB instance 0 ---\n";
    uiBridge.sendModelBCommand(0, 2000);

    return 0;
}
