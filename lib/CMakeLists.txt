cmake_minimum_required (VERSION 3.8)

set(modbus_public_headers
    ./include/modbus/IModbusManager.h

    ./include/modbus/logging/LoggerFactory.h
)

set(modbus_private_headers
    ./src/ModbusManagerImpl.h
    ./src/PrivateHeader.h

    ./src/logging/Logger.h
)

set(modbus_src
    ./src/IModbusManager.cpp
    ./src/ModbusManagerImpl.cpp
    ./src/PrivateHeader.cpp

    ./src/logging/LoggerFactory.cpp
)

add_library(modbus ${modbus_public_headers} ${modbus_private_headers} ${modbus_src})
target_compile_features(modbus PRIVATE cxx_std_14)
target_link_libraries(modbus
    PUBLIC openpal
    PRIVATE asio spdlog
)
target_include_directories(modbus
    PUBLIC ./include
    PRIVATE ./src
)

add_subdirectory(./tests)
