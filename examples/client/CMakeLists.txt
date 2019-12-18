cmake_minimum_required (VERSION 3.8)

set(modbus_client_example_src
    ./main.cpp
)

add_executable(modbus_client_example ${modbus_client_example_src})
target_compile_features(modbus_client_example PRIVATE cxx_std_14)
target_link_libraries(modbus_client_example PRIVATE modbus-cpp)
