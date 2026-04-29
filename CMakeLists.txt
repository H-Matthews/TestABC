cmake_minimum_required(VERSION 3.16)
project(DataCache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(data_cache
    main.cpp
    cache/DataCache.cpp
    UIBridge.cpp
    ../SignalDispathcer/SignalDispatcher.cpp
)

target_include_directories(data_cache PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cache
    ${CMAKE_CURRENT_SOURCE_DIR}/cache/Signals
    ${CMAKE_CURRENT_SOURCE_DIR}/cache/State
    ${CMAKE_CURRENT_SOURCE_DIR}/cache/State/Base
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelA
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelA/Signals
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelB
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelB/Signals
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelC
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelC/Signals
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelD
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelD/Signals
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelE
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelE/Signals
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelF
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelF/Signals
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelG
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelG/Signals
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelH
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelH/Signals
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelI
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelI/Signals
    ${CMAKE_CURRENT_SOURCE_DIR}/../SignalDispathcer
)
