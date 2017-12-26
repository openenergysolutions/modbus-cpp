cmake_minimum_required (VERSION 3.8)

set(modbus_example_src
    ./main.cpp
)

add_executable(modbus_example ${modbus_example_src})
target_link_libraries(modbus_example
    PRIVATE modbus
)
