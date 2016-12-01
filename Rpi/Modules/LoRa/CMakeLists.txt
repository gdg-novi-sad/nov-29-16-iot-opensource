include_directories (${CMAKE_SOURCE_DIR}/out/include)
include_directories (${CMAKE_SOURCE_DIR}/Modules/LoRa/inc)
link_directories(${CMAKE_SOURCE_DIR}/out/lib)

set(LoRa_source LoRa.cpp)
add_executable(LoRa ${LoRa_source})

target_link_libraries(LoRa lora pthread mqttpp paho-mqtt3as core  PocoNet PocoNetSSL PocoCrypto PocoUtil PocoFoundation PocoJSON PocoXML PocoSerial)

