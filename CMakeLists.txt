cmake_minimum_required(VERSION 3.16)
project(DataCache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(data_cache
    main.cpp
    DataCache.cpp
    UIBridge.cpp
    ../SignalDispathcer/SignalDispatcher.cpp
)

target_include_directories(data_cache PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../SignalDispathcer
)
