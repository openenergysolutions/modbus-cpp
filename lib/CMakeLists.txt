cmake_minimum_required (VERSION 3.8)

set(modbus_public_headers
    ./include/modbus/Expected.h
    ./include/modbus/IModbusManager.h
    ./include/modbus/Ipv4Endpoint.h
    ./include/modbus/ITimer.h
    ./include/modbus/Typedefs.h
    ./include/modbus/UnitIdentifier.h

    ./include/modbus/client/IClientChannel.h
    ./include/modbus/client/IClientSession.h
    ./include/modbus/client/IScheduledRequest.h
    ./include/modbus/client/ResponseHandler.h

    ./include/modbus/exceptions/ConnectionException.h
    ./include/modbus/exceptions/IException.h
    ./include/modbus/exceptions/MalformedModbusRequestException.h
    ./include/modbus/exceptions/MalformedModbusResponseException.h
    ./include/modbus/exceptions/ModbusException.h
    ./include/modbus/exceptions/TimeoutException.h

    ./include/modbus/logging/LoggerFactory.h
    ./include/modbus/logging/LoggingLevel.h

    ./include/modbus/messages/Address.h
    ./include/modbus/messages/ExceptionType.h
    ./include/modbus/messages/ReadCoilsRequest.h
    ./include/modbus/messages/ReadCoilsResponse.h
    ./include/modbus/messages/ReadDiscreteInputsRequest.h
    ./include/modbus/messages/ReadDiscreteInputsResponse.h
    ./include/modbus/messages/ReadHoldingRegistersRequest.h
    ./include/modbus/messages/ReadHoldingRegistersResponse.h
    ./include/modbus/messages/ReadInputRegistersRequest.h
    ./include/modbus/messages/ReadInputRegistersResponse.h
    ./include/modbus/messages/RegisterValue.h
    ./include/modbus/messages/SingleBitValue.h
    ./include/modbus/messages/WriteMultipleCoilsRequest.h
    ./include/modbus/messages/WriteMultipleCoilsResponse.h
    ./include/modbus/messages/WriteMultipleRegistersRequest.h
    ./include/modbus/messages/WriteMultipleRegistersResponse.h
    ./include/modbus/messages/WriteSingleCoilRequest.h
    ./include/modbus/messages/WriteSingleCoilResponse.h
    ./include/modbus/messages/WriteSingleRegisterRequest.h
    ./include/modbus/messages/WriteSingleRegisterResponse.h

    ./include/modbus/server/IServerChannel.h
    ./include/modbus/server/IServerSession.h
)

set(modbus_private_headers
    ./src/IConnectionListener.h
    ./src/ITcpConnection.h
    ./src/ModbusManagerImpl.h
    ./src/TimerWrapper.h
    ./src/TransactionIdentifier.h

    ./src/client/channel/AsioClientTcpConnection.h
    ./src/client/channel/ClientChannelTcp.h
    ./src/client/channel/IClientChannelImpl.h
    ./src/client/channel/PendingRequest.h

    ./src/client/session/ClientSessionImpl.h
    ./src/client/session/ScheduledRequest.h

    ./src/logging/Logger.h
    ./src/logging/LoggingLevelConversion.h

    ./src/messages/IMessage.h
    ./src/messages/ReadBitsRequestImpl.h
    ./src/messages/ReadBitsResponseImpl.h
    ./src/messages/ReadCoilsRequestImpl.h
    ./src/messages/ReadCoilsResponseImpl.h
    ./src/messages/ReadDiscreteInputsRequestImpl.h
    ./src/messages/ReadDiscreteInputsResponseImpl.h
    ./src/messages/ReadHoldingRegistersRequestImpl.h
    ./src/messages/ReadHoldingRegistersResponseImpl.h
    ./src/messages/ReadInputRegistersRequestImpl.h
    ./src/messages/ReadInputRegistersResponseImpl.h
    ./src/messages/ReadRegistersRequestImpl.h
    ./src/messages/ReadRegistersResponseImpl.h
    ./src/messages/WriteMultipleCoilsRequestImpl.h
    ./src/messages/WriteMultipleCoilsResponseImpl.h
    ./src/messages/WriteMultipleRegistersRequestImpl.h
    ./src/messages/WriteMultipleRegistersResponseImpl.h
    ./src/messages/WriteSingleCoilRequestImpl.h
    ./src/messages/WriteSingleCoilResponseImpl.h
    ./src/messages/WriteSingleRegisterRequestImpl.h
    ./src/messages/WriteSingleRegisterResponseImpl.h

    ./src/messages/mbap/IMbapSink.h
    ./src/messages/mbap/MbapMessage.h
    ./src/messages/mbap/MbapParser.h

    ./src/server/channel/AsioServer.h
    ./src/server/channel/AsioServerTcpConnection.h
    ./src/server/channel/IServer.h
    ./src/server/channel/IServerChannelImpl.h
    ./src/server/channel/ServerChannelTcp.h
    ./src/server/channel/ServerConnectionListener.h
    ./src/server/channel/ServerConnectionListenerBuilder.h
)

set(modbus_src
    ./src/IModbusManager.cpp
    ./src/Ipv4Endpoint.cpp
    ./src/ModbusManagerImpl.cpp
    ./src/TimerWrapper.cpp
    ./src/TransactionIdentifier.cpp
    ./src/UnitIdentifier.cpp

    ./src/client/channel/AsioClientTcpConnection.cpp
    ./src/client/channel/ClientChannelTcp.cpp

    ./src/client/session/ClientSessionImpl.cpp
    ./src/client/session/ScheduledRequest.hpp

    ./src/logging/LoggerFactory.cpp

    ./src/messages/ExceptionType.cpp
    ./src/messages/IMessage.cpp
    ./src/messages/ReadBitsRequestImpl.cpp
    ./src/messages/ReadBitsResponseImpl.cpp
    ./src/messages/ReadRegistersRequestImpl.cpp
    ./src/messages/ReadRegistersResponseImpl.cpp
    ./src/messages/WriteMultipleCoilsRequestImpl.cpp
    ./src/messages/WriteMultipleCoilsResponseImpl.cpp
    ./src/messages/WriteMultipleRegistersRequestImpl.cpp
    ./src/messages/WriteMultipleRegistersResponseImpl.cpp
    ./src/messages/WriteSingleCoilRequestImpl.cpp
    ./src/messages/WriteSingleCoilResponseImpl.cpp
    ./src/messages/WriteSingleRegisterRequestImpl.cpp
    ./src/messages/WriteSingleRegisterResponseImpl.cpp

    ./src/messages/mbap/MbapMessage.cpp
    ./src/messages/mbap/MbapParser.cpp

    ./src/server/channel/AsioServer.cpp
    ./src/server/channel/AsioServerTcpConnection.cpp
    ./src/server/channel/ServerChannelTcp.cpp
    ./src/server/channel/ServerConnectionListener.cpp
    ./src/server/channel/ServerConnectionListenerBuilder.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${modbus_public_headers} ${modbus_private_headers} ${modbus_src})

add_library(modbus-cpp ${modbus_public_headers} ${modbus_private_headers} ${modbus_src})
target_compile_features(modbus-cpp PRIVATE cxx_std_14)
target_link_libraries(modbus-cpp PRIVATE asio spdlog exe4cpp ser4cpp)
target_include_directories(modbus-cpp
    PUBLIC ./include
    PRIVATE ./src
)

if(${MODBUS_BUILD_TESTS})
    add_subdirectory(./tests)
endif()
