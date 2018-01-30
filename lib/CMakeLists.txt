cmake_minimum_required (VERSION 3.8)

set(modbus_public_headers
    ./include/modbus/Expected.h
    ./include/modbus/IModbusManager.h
    ./include/modbus/Ipv4Endpoint.h
    ./include/modbus/ISchedule.h
    ./include/modbus/ISessionResponseHandler.h
    ./include/modbus/ResponseHandler.h
    ./include/modbus/ScheduleFactory.h

    ./include/modbus/channel/IChannel.h
    ./include/modbus/channel/UnitIdentifier.h

    ./include/modbus/exceptions/ConnectionException.h
    ./include/modbus/exceptions/IException.h
    ./include/modbus/exceptions/MalformedModbusResponseException.h
    ./include/modbus/exceptions/ModbusException.h
    ./include/modbus/exceptions/TimeoutException.h

    ./include/modbus/logging/LoggerFactory.h

    ./include/modbus/messages/Address.h
    ./include/modbus/messages/ExceptionType.h
    ./include/modbus/messages/IRequest.h
    ./include/modbus/messages/IResponse.h
    ./include/modbus/messages/ReadHoldingRegistersRequest.h
    ./include/modbus/messages/ReadHoldingRegistersResponse.h
    ./include/modbus/messages/ReadInputRegistersRequest.h
    ./include/modbus/messages/ReadInputRegistersResponse.h
    ./include/modbus/messages/RegisterValue.h
    ./include/modbus/messages/WriteSingleRegisterRequest.h
    ./include/modbus/messages/WriteSingleRegisterResponse.h

    ./include/modbus/session/ISession.h
)

set(modbus_private_headers
    ./src/ModbusManagerImpl.h
    ./src/PeriodicSchedule.h

    ./src/channel/AsioTcpConnection.h
    ./src/channel/ChannelTcp.h
    ./src/channel/IConnectionListener.h
    ./src/channel/IMbapSink.h
    ./src/channel/ITcpConnection.h
    ./src/channel/MbapMessage.h
    ./src/channel/MbapParser.h
    ./src/channel/PendingRequest.h
    ./src/channel/TransactionIdentifier.h

    ./src/logging/Logger.h

    ./src/session/SessionImpl.h
)

set(modbus_src
    ./src/IModbusManager.cpp
    ./src/Ipv4Endpoint.cpp
    ./src/ModbusManagerImpl.cpp
    ./src/PeriodicSchedule.cpp
    ./src/ScheduleFactory.cpp

    ./src/channel/ChannelTcp.cpp
    ./src/channel/AsioTcpConnection.cpp
    ./src/channel/MbapMessage.cpp
    ./src/channel/MbapParser.cpp
    ./src/channel/TransactionIdentifier.cpp
    ./src/channel/UnitIdentifier.cpp

    ./src/logging/LoggerFactory.cpp

    ./src/messages/ExceptionType.cpp
    ./src/messages/IResponse.cpp
    ./src/messages/ReadHoldingRegistersRequest.cpp
    ./src/messages/ReadHoldingRegistersResponse.cpp
    ./src/messages/ReadInputRegistersRequest.cpp
    ./src/messages/ReadInputRegistersResponse.cpp
    ./src/messages/WriteSingleRegisterRequest.cpp
    ./src/messages/WriteSingleRegisterResponse.cpp

    ./src/session/SessionImpl.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${modbus_public_headers} ${modbus_private_headers} ${modbus_src})

add_library(modbus ${modbus_public_headers} ${modbus_private_headers} ${modbus_src})
target_compile_features(modbus PRIVATE cxx_std_14)
target_link_libraries(modbus
    PUBLIC openpal
    PRIVATE asio asiopal spdlog
)
target_include_directories(modbus
    PUBLIC ./include
    PRIVATE ./src
)

if(${MODBUS_BUILD_TESTS})
    add_subdirectory(./tests)
endif()
