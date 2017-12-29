cmake_minimum_required (VERSION 3.8)

set(modbus_public_headers
    ./include/modbus/IModbusManager.h

    ./include/modbus/logging/LoggerFactory.h
    ./include/modbus/IChannel.h include/modbus/messages/ReadHoldingRegistersRequest.h include/modbus/messages/IRequest.h include/modbus/messages/Address.h include/modbus/messages/ReadInputRegistersRequest.h include/modbus/messages/IResponse.h include/modbus/messages/ReadHoldingRegistersResponse.h include/modbus/ISession.h include/modbus/ResponseHandler.h include/modbus/messages/ExceptionType.h include/modbus/messages/Exception.h include/modbus/messages/ReadInputRegistersResponse.h include/modbus/UnitIdentifier.h include/modbus/ISchedule.h include/modbus/messages/RegisterValue.h include/modbus/ISessionResponseHandler.h)

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
    src/messages/ExceptionType.cpp src/messages/Exception.cpp src/messages/ReadHoldingRegistersRequest.cpp src/messages/ReadHoldingRegistersResponse.cpp src/UnitIdentifier.cpp)

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
