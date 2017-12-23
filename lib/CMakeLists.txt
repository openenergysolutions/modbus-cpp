cmake_minimum_required (VERSION 3.8)

set(modbus_public_headers
    ./include/modbus/ModbusManager.h
)

set(modbus_private_headers
    ./src/PrivateHeader.h
)

set(modbus_src
    ./src/ModbusManager.cpp
    ./src/PrivateHeader.cpp
)

add_library(modbus ${modbus_public_headers} ${modbus_private_headers} ${modbus_src})
target_link_libraries(modbus
    PUBLIC openpal
    PRIVATE asio
)
target_include_directories(modbus
    PUBLIC ./include
    PRIVATE ./src
)

add_subdirectory(./tests)
