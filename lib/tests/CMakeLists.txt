cmake_minimum_required (VERSION 3.8)

set(modbus_tests_src
    ./main.cpp
    ./Ipv4EndpointTest.cpp
    ./PeriodicScheduleTest.cpp

    ./channel/TransactionIdentifierTest.cpp
    ./channel/UnitIdentifierTest.cpp

    ./session/SessionImplTest.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${modbus_tests_src})

add_executable(modbus_tests ${modbus_tests_src})
target_link_libraries(modbus_tests
    PRIVATE catch2 fakeit modbus
)
target_include_directories(modbus_tests
    PRIVATE ./ ../src
)

add_test(modbus_tests modbus_tests)
