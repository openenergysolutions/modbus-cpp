cmake_minimum_required (VERSION 3.8)

set(modbus_tests_headers
    ./mocks/AsioTcpConnectionWrapper.h
    ./mocks/ConnectionListenerMock.h
    ./mocks/ExecutorMock.h
    ./mocks/IResponseMock.h
    ./mocks/ISessionMock.h
    ./mocks/ISessionResponseHandlerMock.h
    ./mocks/MbapSinkMock.h
    ./mocks/RequestMock.h
    ./mocks/TcpConnectionMock.h
    ./mocks/TestServer.h
    ./mocks/TimerMock.h
)

set(modbus_tests_src
    ./main.cpp
    ./Ipv4EndpointTest.cpp
    ./PeriodicScheduleTest.cpp

    ./channel/AsioTcpConnectionTest.cpp
    ./channel/ChannelTcpTest.cpp
    ./channel/MbapMessageTest.cpp
    ./channel/MbapParserTest.cpp
    ./channel/TransactionIdentifierTest.cpp
    ./channel/UnitIdentifierTest.cpp

    ./messages/ExceptionTypeTest.cpp
    ./messages/IResponseTest.cpp
    ./messages/ReadHoldingRegistersRequestTest.cpp
    ./messages/ReadHoldingRegistersResponseTest.cpp
    ./messages/ReadInputRegistersRequestTest.cpp
    ./messages/ReadInputRegistersResponseTest.cpp
    ./messages/WriteMultipleRegistersRequestTest.cpp
    ./messages/WriteMultipleRegistersResponseTest.cpp
    ./messages/WriteSingleRegisterRequestTest.cpp
    ./messages/WriteSingleRegisterResponseTest.cpp

    ./mocks/AsioTcpConnectionWrapper.cpp
    ./mocks/ConnectionListenerMock.cpp
    ./mocks/ExecutorMock.cpp
    ./mocks/IResponseMock.cpp
    ./mocks/ISessionMock.cpp
    ./mocks/ISessionResponseHandlerMock.cpp
    ./mocks/MbapSinkMock.cpp
    ./mocks/RequestMock.cpp
    ./mocks/TcpConnectionMock.cpp
    ./mocks/TestServer.cpp
    ./mocks/TimerMock.cpp

    ./session/ScheduledRequestTest.cpp
    ./session/SessionImplTest.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${modbus_tests_headers} ${modbus_tests_src})

add_executable(modbus_tests ${modbus_tests_headers} ${modbus_tests_src})
target_link_libraries(modbus_tests
    PRIVATE catch2 modbus openpal asiopal asio spdlog
)
target_include_directories(modbus_tests
    PRIVATE ./ ../src
)

add_test(modbus_tests modbus_tests)
