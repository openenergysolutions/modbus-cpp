cmake_minimum_required (VERSION 3.8)

set(modbus_server_example_src
    ./main.cpp
)

add_executable(modbus_server_example ${modbus_server_example_src})
target_compile_features(modbus_server_example PRIVATE cxx_std_14)
target_link_libraries(modbus_server_example PRIVATE modbus-cpp)
