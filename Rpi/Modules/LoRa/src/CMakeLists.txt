
set(lora_source_files
  LoRaEngine.cpp
)

set (lora_header_files
  ../inc/LoRaEngine.h
)

include_directories (${CMAKE_SOURCE_DIR}/out/include)
include_directories (${CMAKE_SOURCE_DIR}/Modules/LoRa/inc/)

add_library(lora ${lora_source_files} ${lora_header_files})

target_link_libraries(lora pthread  PocoNet PocoNetSSL PocoCrypto PocoUtil PocoFoundation PocoJSON PocoXML PocoData PocoDataSQLite PocoSerial)


