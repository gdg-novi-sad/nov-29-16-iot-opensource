include_directories (${CMAKE_SOURCE_DIR}/IoTclient/core/inc)
include_directories (${CMAKE_SOURCE_DIR}/out/include)
include_directories (${CMAKE_SOURCE_DIR}/IoTclient/wolkconnect/inc)
link_directories(${CMAKE_SOURCE_DIR}/out/lib)

set(IoT_files IoTclient.cpp)
add_executable(IoTclient ${IoT_files})

target_link_libraries(IoTclient pthread mqttpp paho-mqtt3as core  PocoNet PocoNetSSL PocoCrypto PocoUtil PocoFoundation PocoJSON PocoXML PocoSerial)

