
set(core_source_files
  Util.cpp
  Ipc.cpp
)

set (core_header_files
  ../inc/Util.h
  ../inc/Ipc.h
)

include_directories (${CMAKE_SOURCE_DIR}/out/include)
include_directories (${CMAKE_SOURCE_DIR}/IoTclient/wolkconnect/inc)
include_directories (${CMAKE_SOURCE_DIR}/IoTclient/core/inc)

add_library(core ${core_source_files} ${core_header_files})

target_link_libraries(core paho-mqtt3as mqttpp pthread  PocoNet PocoNetSSL PocoCrypto PocoUtil PocoFoundation PocoJSON PocoXML PocoData PocoDataSQLite wolkconnect)


