cmake_minimum_required(VERSION 2.8)

# Options. Turn on with 'cmake -Dmyvarname=ON'.
option(test "Build all tests." OFF) # Makes boolean 'test' available.

set(PROJECT_NAME IoTplatform)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "-g -Wall")

add_definitions(-std=c++11)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/bin/")

add_subdirectory(IoTclient)
add_subdirectory(IoTclient/core/src)
add_subdirectory(IoTclient/wolkconnect/src)

add_subdirectory(Modules/LoRa)
add_subdirectory(Modules/LoRa/src)

