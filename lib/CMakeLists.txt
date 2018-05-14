cmake_minimum_required (VERSION 3.8)

set(modbus_public_headers
    ./include/modbus/Expected.h
    ./include/modbus/IModbusManager.h
    ./include/modbus/ITimer.h
    ./include/modbus/ResponseHandler.h
    ./include/modbus/Typedefs.h

    ./include/modbus/channel/IChannel.h
    ./include/modbus/channel/Ipv4Endpoint.h
    ./include/modbus/channel/UnitIdentifier.h

    ./include/modbus/exceptions/ConnectionException.h
    ./include/modbus/exceptions/IException.h
    ./include/modbus/exceptions/MalformedModbusResponseException.h
    ./include/modbus/exceptions/ModbusException.h
    ./include/modbus/exceptions/TimeoutException.h

    ./include/modbus/logging/LoggerFactory.h

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

    ./include/modbus/session/IScheduledRequest.h
    ./include/modbus/session/ISession.h
)

set(modbus_private_headers
    ./src/ModbusManagerImpl.h
    ./src/TimerWrapper.h

    ./src/channel/AsioTcpConnection.h
    ./src/channel/ChannelTcp.h
    ./src/channel/IChannelImpl.h
    ./src/channel/IConnectionListener.h
    ./src/channel/IMbapSink.h
    ./src/channel/ITcpConnection.h
    ./src/channel/MbapMessage.h
    ./src/channel/MbapParser.h
    ./src/channel/PendingRequest.h
    ./src/channel/TransactionIdentifier.h

    ./src/logging/Logger.h

    ./src/messages/IRequest.h
    ./src/messages/IResponse.h
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

    ./src/session/ScheduledRequest.h
    ./src/session/SessionImpl.h
)

set(modbus_src
    ./src/IModbusManager.cpp
    ./src/ModbusManagerImpl.cpp
    ./src/TimerWrapper.cpp

    ./src/channel/AsioTcpConnection.cpp
    ./src/channel/ChannelTcp.cpp
    ./src/channel/Ipv4Endpoint.cpp
    ./src/channel/MbapMessage.cpp
    ./src/channel/MbapParser.cpp
    ./src/channel/TransactionIdentifier.cpp
    ./src/channel/UnitIdentifier.cpp

    ./src/logging/LoggerFactory.cpp

    ./src/messages/ExceptionType.cpp
    ./src/messages/IResponse.cpp
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

    ./src/session/ScheduledRequest.hpp
    ./src/session/SessionImpl.cpp
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
