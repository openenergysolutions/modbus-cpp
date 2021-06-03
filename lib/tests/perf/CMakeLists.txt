cmake_minimum_required (VERSION 3.8)

set(modbus_perf_tests_headers
    ./ServerSession.h
)

set(modbus_perf_tests_src
    ./main.cpp
    ./ServerSession.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${modbus_perf_tests_src})

add_executable(modbus_perf_tests ${modbus_perf_tests_src})
target_link_libraries(modbus_perf_tests
    PRIVATE modbus-cpp spdlog::spdlog
)
target_include_directories(modbus_perf_tests
    PRIVATE ./
)
