cmake_minimum_required (VERSION 3.8)

project (modbus-cpp)

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
set(MODBUS_BUILD_EXAMPLES ${is_root} CACHE BOOL "Build examples")

# Import dependencies
include(./deps/asio.cmake)
include(./deps/ser4cpp.cmake)
include(./deps/exe4cpp.cmake)
include(./deps/spdlog.cmake)
if(${MODBUS_BUILD_TESTS})
    include(./deps/catch.cmake)
    include(./deps/trompeloeil.cmake)
endif()

# Add projects
add_subdirectory(./lib)

if(${MODBUS_BUILD_EXAMPLES})
    add_subdirectory(./examples)
endif()

# Custom target for documentation and license
find_package(PythonInterp 3)
if(PYTHONINTERP_FOUND)
    add_custom_target(
        doc
        COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/deps/m.css/doxygen/dox2html5.py" "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile-mcss"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Build documentation"
    )

    add_custom_target(
        license_header
        COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/deps/licenseheaders/licenseheaders.py" -d "${CMAKE_CURRENT_SOURCE_DIR}/lib" -t "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md"
        COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/deps/licenseheaders/licenseheaders.py" -d "${CMAKE_CURRENT_SOURCE_DIR}/example"  -t "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md"
        COMMENT "Add license to headers files"
    )
endif()
