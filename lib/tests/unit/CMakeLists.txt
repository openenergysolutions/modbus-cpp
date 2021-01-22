cmake_minimum_required (VERSION 3.8)

set(modbus_tests_headers
    ./mocks/AsioClientTcpConnectionWrapper.h
    ./mocks/AsioServerWrapper.h
    ./mocks/ConnectionListenerMock.h
    ./mocks/IClientSessionMock.h
    ./mocks/IMessageMock.h
    ./mocks/IServerChannelImplMock.h
    ./mocks/IServerConnectionListenerBuilderMock.h
    ./mocks/IServerMock.h
    ./mocks/IServerSessionMock.h
    ./mocks/MbapSinkMock.h
    ./mocks/TcpConnectionMock.h
    ./mocks/TestServer.h
)

set(modbus_tests_src
    ./main.cpp

    ./Ipv4EndpointTest.cpp
    ./TransactionIdentifierTest.cpp
    ./UnitIdentifierTest.cpp

    ./client/channel/AsioClientTcpConnectionTest.cpp
    ./client/channel/ClientChannelTcpTest.cpp

    ./client/session/ScheduledRequestTest.cpp

    ./messages/ExceptionResponseTest.cpp
    ./messages/ExceptionTypeTest.cpp
    ./messages/IMessageTest.cpp
    ./messages/ReadCoilsRequestImplTest.cpp
    ./messages/ReadCoilsResponseImplTest.cpp
    ./messages/ReadDiscreteInputsRequestImplTest.cpp
    ./messages/ReadDiscreteInputsResponseImplTest.cpp
    ./messages/ReadHoldingRegistersRequestImplTest.cpp
    ./messages/ReadHoldingRegistersResponseImplTest.cpp
    ./messages/ReadInputRegistersRequestImplTest.cpp
    ./messages/ReadInputRegistersResponseImplTest.cpp
    ./messages/WriteMultipleCoilsRequestImplTest.cpp
    ./messages/WriteMultipleCoilsResponseImplTest.cpp
    ./messages/WriteMultipleRegistersRequestImplTest.cpp
    ./messages/WriteMultipleRegistersResponseImplTest.cpp
    ./messages/WriteSingleCoilRequestImplTest.cpp
    ./messages/WriteSingleCoilResponseImplTest.cpp
    ./messages/WriteSingleRegisterRequestImplTest.cpp
    ./messages/WriteSingleRegisterResponseImplTest.cpp

    ./messages/mbap/MbapMessageTest.cpp
    ./messages/mbap/MbapParserTest.cpp

    ./mocks/AsioClientTcpConnectionWrapper.cpp
    ./mocks/AsioServerWrapper.cpp
    ./mocks/ConnectionListenerMock.cpp
    ./mocks/IMessageMock.cpp
    ./mocks/IClientSessionMock.cpp
    ./mocks/IServerChannelImplMock.cpp
    ./mocks/IServerConnectionListenerBuilderMock.cpp
    ./mocks/MbapSinkMock.cpp
    ./mocks/TcpConnectionMock.cpp
    ./mocks/TestServer.cpp

    ./server/channel/AsioServerTest.cpp
    ./server/channel/ServerChannelTcpTest.cpp
    ./server/channel/ServerConnectionListenerBuilderTest.cpp
    ./server/channel/ServerConnectionListenerTest.cpp

    ./server/db/InMemoryDatabaseTest.cpp
    ./server/db/SingleTypeDatabaseTest.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${modbus_tests_headers} ${modbus_tests_src})

add_executable(modbus_tests ${modbus_tests_headers} ${modbus_tests_src})
target_link_libraries(modbus_tests PRIVATE modbus-cpp Catch2::Catch2 trompeloeil spdlog exe4cpp ser4cpp asio)

target_include_directories(modbus_tests
    PRIVATE ./ ../../src
)

add_test(modbus_tests modbus_tests)
