cmake_minimum_required (VERSION 3.8)

set(modbus_tests_src
    ./main.cpp
    ./Ipv4EndpointTest.cpp
    ./PeriodicScheduleTest.cpp
    ./UnitIdentifierTest.cpp
)

add_executable(modbus_tests ${modbus_tests_src})
target_link_libraries(modbus_tests
    PRIVATE catch2 modbus
)
target_include_directories(modbus_tests
    PRIVATE ./ ../src
)

add_test(modbus_tests modbus_tests)
