set(core_source_files
  Device.cpp
  PublishingService.cpp
  WebClient.cpp
  WebClientException.cpp
)

include_directories (${CMAKE_SOURCE_DIR}/out/include)
include_directories (${CMAKE_SOURCE_DIR}/IoTclient/core/inc)
include_directories (${CMAKE_SOURCE_DIR}/IoTclient/wolkconnect/inc)

add_library(wolkconnect ${core_source_files})

target_link_libraries(wolkconnect crypto ssl PocoNetSSL PocoCrypto PocoNet PocoUtil PocoJSON PocoXML PocoFoundation)
