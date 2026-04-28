# TestABC — System Description

**Repository:** [H-Matthews/TestABC](https://github.com/H-Matthews/TestABC)  
**Language:** C++17  
**Build System:** CMake (minimum version 3.16)  
**Project Name:** `DataCache`  
**Executable:** `data_cache`

---

## Overview

TestABC is a C++17 simulation framework that demonstrates a **delta-driven data cache architecture** connecting multiple simulation model components to a UI layer through a decoupled signal dispatcher. The system separates concerns into three distinct layers: model components that produce data, a central data cache that assembles partial updates, and a UI bridge that consumes complete snapshots and issues commands back to the models.

The design is intended to support real-time simulation scenarios where different subsystems report telemetry fields independently and asynchronously. The cache acts as an accumulation buffer, firing a callback only once all required fields for a given model instance have been received — avoiding partial or inconsistent state from reaching the UI.

---

## Architecture

```
┌──────────────────────────────────────────────────────────┐
│                        main.cpp                          │
│   Wires together dispatcher, UIBridge, and model         │
│   components. Drives push/query/command flows.           │
└────────────────┬──────────────────────┬─────────────────┘
                 │                      │
        constructs/owns         constructs/owns
                 │                      │
    ┌────────────▼──────┐   ┌───────────▼──────────────┐
    │    UIBridge        │   │   ModelAComponent (×N)   │
    │   (namespace dc)   │   │   ModelBComponent (×N)   │
    │                    │   │   (namespace dc)          │
    │  - Owns DataCache  │   │  - Broadcast deltas via   │
    │  - Registers delta │   │    SignalDispatcher       │
    │    handlers        │   │  - Receive commands via   │
    │  - Exposes query   │   │    SignalDispatcher       │
    │    API             │   └───────────────────────────┘
    │  - Sends commands  │
    └────────┬───────────┘
             │ owns
    ┌────────▼───────────┐
    │     DataCache       │
    │   (namespace dc)    │
    │                     │
    │  - Type-keyed store │
    │  - Bitmask field    │
    │    accumulation     │
    │  - Fires callback   │
    │    when all fields  │
    │    are populated    │
    └─────────────────────┘
             ▲
             │ signals routed through
    ┌────────┴───────────┐
    │  SignalDispatcher   │
    │   (namespace sd)    │
    │                     │
    │  - Topic-based pub/ │
    │    sub messaging    │
    │  - Decouples models │
    │    from UIBridge    │
    └─────────────────────┘
```

---

## Core Components

### 1. `SignalDispatcher` (`sd` namespace)

Located in an adjacent `SignalDispatcher/` directory (outside `TestABC/`), this is the central message bus. Components publish signals to string-keyed topics (e.g., `"model_a.0"`) and subscribers register handlers for those topics. It decouples producers (model components) from consumers (UIBridge/DataCache) so neither side depends directly on the other.

### 2. `DataCache` (`dc` namespace)

The accumulation engine of the system. It maintains a type-keyed, instance-indexed store of partially assembled model states. Each model type has an associated bitmask that tracks which fields have been received. When all required fields for a given instance are populated, the DataCache fires a user-supplied callback with a fully assembled, immutable `shared_ptr<const TState>` snapshot. Key behaviors:

- **`reserve<TState>(count)`** — Pre-allocates storage for a fixed number of model instances before simulation starts.
- **`addInstance<TState>(index)`** — Dynamically registers a new instance at runtime, safe to call during an active simulation.
- **`query<TState>(index)`** — Performs an on-demand synchronous lookup, returning a `QueryResult` with a `Ready` or `UnknownIndex` status.
- **`registerHandlers(dispatcher)`** — Subscribes the DataCache's delta-merge logic to the SignalDispatcher so incoming delta signals are automatically processed.
- **`setCallback<TState>(...)`** — Registers the callback that fires when an instance's state is complete.

### 3. `UIBridge` (`dc` namespace)

The interface between the simulation backend and the UI layer. It owns a `DataCache` instance, configures it, and exposes a clean API for both data retrieval and command dispatch.

**Responsibilities:**
- Wires DataCache callbacks on construction so that completed model snapshots are forwarded to `onModelAReady` / `onModelBReady`.
- Provides `queryModelA(index)` and `queryModelB(index)` for synchronous on-demand state access, bypassing the signal pipeline.
- Provides `sendModelACommand(index, temperature)` and `sendModelBCommand(index, rpm)` to send typed command signals back to specific model instances via the dispatcher.

UIBridge is non-copyable to enforce single ownership of the DataCache.

### 4. Model Components (`dc` namespace)

**`ModelAComponent`** and **`ModelBComponent`** represent simulation actors. Each instance is identified by a numeric index and registers its own command handler with the dispatcher upon construction. They publish partial state updates ("deltas") via `broadcastDelta(...)`, specifying which fields are included using bitmask flags.

**ModelA Fields (`ModelAField` enum):**
| Field | Type | Description |
|-------------|-------|-------------|
| Temperature | float | Thermal measurement |
| Pressure | float | Pressure reading |
| FlowRate | float | Flow measurement |
| Voltage | float | Electrical reading |

**ModelB Fields (`ModelBField` enum):**
| Field | Type | Description |
|-------|------|-------------|
| Rpm | int | Rotational speed |
| Torque | float | Torque measurement |

A convenience constant `kModelAAllFields` allows broadcasting all ModelA fields in a single call.

### 5. `QueryResult<TState>`

A lightweight result type returned by on-demand queries. Contains a status enum (`Ready` or `UnknownIndex`) and, when ready, a `shared_ptr<const TState>` snapshot for safe read access.

---

## Data Flow

### Push Flow (Delta → Snapshot → Callback)

1. A model component calls `broadcastDelta(...)` with a partial set of field values and a bitmask indicating which fields are present.
2. The signal is published to the SignalDispatcher on the component's topic (e.g., `"model_a.0"`).
3. The DataCache's registered handler receives the signal, merges the delta into the accumulation buffer for that instance, and updates the field-seen bitmask.
4. If all required fields have now been observed, the DataCache assembles an immutable snapshot and invokes the UIBridge callback.
5. UIBridge logs (or otherwise presents) the completed state to the UI.

> **Example from `main.cpp`:** ModelA instance 0 first broadcasts `Temperature + Pressure` — no callback yet. Then it broadcasts `FlowRate + Voltage` — all four fields now seen, so the callback fires.

### Query Flow (On-Demand Pull)

1. UIBridge calls `queryModelA(index)` or `queryModelB(index)`.
2. DataCache looks up the instance by index and returns a `QueryResult`.
3. If the instance exists and all fields are populated, status is `Ready` with a snapshot pointer.
4. If the index is unrecognized, status is `UnknownIndex`.

### Command Flow (UI → Model)

1. UIBridge calls `sendModelACommand(index, value)` or `sendModelBCommand(index, value)`.
2. UIBridge constructs a typed command signal (`ModelACommandSignal` / `ModelBCommandSignal`) and dispatches it to the target topic (`"model_a.<index>"` or `"model_b.<index>"`).
3. The target model component's command handler (registered at construction) receives and processes the command.

---

## Directory Structure

```
TestABC/
├── CMakeLists.txt               # Build configuration (project: DataCache)
├── main.cpp                     # Entry point; wires and exercises all flows
├── UIBridge.h / UIBridge.cpp    # UI-facing facade over DataCache
├── cache/
│   ├── DataCache.cpp            # Core accumulation and query logic
│   ├── Signals/                 # Delta signal types for each model
│   └── State/                   # State struct definitions (ModelAState, ModelBState)
│       └── QueryResult.h        # Query result wrapper
├── Models/
│   ├── ModelA/
│   │   ├── ModelAComponent.h/cpp
│   │   └── Signals/             # ModelACommandSignal
│   └── ModelB/
│       ├── ModelBComponent.h/cpp
│       └── Signals/             # ModelBCommandSignal
└── ../SignalDispatcher/         # External (sibling) dispatcher library
    └── SignalDispatcher.cpp
```

---

## Build Instructions

The project uses CMake and requires a C++17-compliant compiler.

```bash
mkdir build && cd build
cmake ..
cmake --build .
./data_cache
```

The `SignalDispatcher` component is expected to reside in a sibling directory relative to `TestABC/` (i.e., `../SignalDispatcher/`).

---

## Key Design Patterns

| Pattern | Where Applied |
|---|---|
| **Observer / Pub-Sub** | SignalDispatcher decouples models from UIBridge |
| **Delta accumulation** | DataCache merges partial updates before firing callbacks |
| **Template-keyed storage** | DataCache uses `TState` type parameter to support multiple model types generically |
| **Immutable snapshots** | Callbacks receive `shared_ptr<const TState>` — safe for concurrent read access |
| **Command pattern** | UIBridge sends typed command signals back to specific model instances |
| **Two-phase initialization** | `reserveInstances` before simulation; `addInstance` for dynamic runtime additions |

---

## Summary

TestABC is a well-structured C++ reference implementation for a **multi-model simulation data pipeline**. Its central idea is that model components can report their state piecemeal (field by field, burst by burst), and the system will automatically assemble complete snapshots and notify the UI only when all data is available. The SignalDispatcher provides clean decoupling between producers and consumers, the DataCache handles the stateful accumulation logic, and the UIBridge presents a high-level interface for both consuming state and issuing commands — making the architecture extensible to additional model types with minimal changes.
