cmake_minimum_required (VERSION 3.8)

set(modbus_public_headers
    ./include/modbus/IChannel.h
    ./include/modbus/IModbusManager.h
    ./include/modbus/Ipv4Endpoint.h
    ./include/modbus/ISession.h
    ./include/modbus/ISchedule.h
    ./include/modbus/ISessionResponseHandler.h
    ./include/modbus/ResponseHandler.h
    ./include/modbus/ScheduleFactory.h
    ./include/modbus/UnitIdentifier.h

    ./include/modbus/logging/LoggerFactory.h

    ./include/modbus/messages/Address.h
    ./include/modbus/messages/Exception.h
    ./include/modbus/messages/ExceptionType.h
    ./include/modbus/messages/IRequest.h
    ./include/modbus/messages/IResponse.h
    ./include/modbus/messages/ReadHoldingRegistersRequest.h
    ./include/modbus/messages/ReadHoldingRegistersResponse.h
    ./include/modbus/messages/ReadInputRegistersRequest.h
    ./include/modbus/messages/ReadInputRegistersResponse.h
    ./include/modbus/messages/RegisterValue.h
)

set(modbus_private_headers
    ./src/ModbusManagerImpl.h
    ./src/PeriodicSchedule.h
    ./src/PrivateHeader.h

    ./src/logging/Logger.h
)

set(modbus_src
    ./src/IModbusManager.cpp
    ./src/Ipv4Endpoint.cpp
    ./src/ModbusManagerImpl.cpp
    ./src/PeriodicSchedule.cpp
    ./src/PrivateHeader.cpp
    ./src/ScheduleFactory.cpp
    ./src/UnitIdentifier.cpp

    ./src/logging/LoggerFactory.cpp

    ./src/messages/ExceptionType.cpp
    ./src/messages/Exception.cpp
    ./src/messages/ReadHoldingRegistersRequest.cpp
    ./src/messages/ReadHoldingRegistersResponse.cpp
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
