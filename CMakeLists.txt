cmake_minimum_required (VERSION 3.8)

project (modbus)

enable_testing()

# Import dependencies
add_subdirectory(./deps/asio EXCLUDE_FROM_ALL)
add_subdirectory(./deps/catch2 EXCLUDE_FROM_ALL)
add_subdirectory(./deps/fakeit EXCLUDE_FROM_ALL)
add_subdirectory(./deps/openpal EXCLUDE_FROM_ALL)
add_subdirectory(./deps/spdlog EXCLUDE_FROM_ALL)

# Add projects
add_subdirectory(./lib)
add_subdirectory(./example)
