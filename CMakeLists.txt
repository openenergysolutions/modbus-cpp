cmake_minimum_required (VERSION 3.8)

project (modbus)

# Preserve folders for Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Check if we are the root project
set(is_root OFF)
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    set(is_root ON)
endif()

# Cache variable to enable unit tests
set(MODBUS_BUILD_TESTS ${is_root} CACHE BOOL "Build unit tests")
if(${MODBUS_BUILD_TESTS})
    enable_testing()
endif()

# Cache variable to enable example
set(MODBUS_BUILD_EXAMPLE ${is_root} CACHE BOOL "Build example")

# Import dependencies
add_subdirectory(./deps/asio EXCLUDE_FROM_ALL)
add_subdirectory(./deps/catch2 EXCLUDE_FROM_ALL)
add_subdirectory(./deps/openpal EXCLUDE_FROM_ALL)
add_subdirectory(./deps/spdlog EXCLUDE_FROM_ALL)

# Add projects
add_subdirectory(./lib)

if(${MODBUS_BUILD_EXAMPLE})
    add_subdirectory(./example)
endif()
