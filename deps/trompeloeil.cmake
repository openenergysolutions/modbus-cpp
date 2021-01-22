if(MODBUS_VENDORED_DEPS)
    include(FetchContent)
    FetchContent_Declare(
        trompeloeil
        GIT_REPOSITORY https://github.com/rollbear/trompeloeil
        GIT_TAG        v39)

    FetchContent_MakeAvailable(trompeloeil)

    #FetchContent_Declare(
    #    trompeloeil
    #    URL                 https://raw.githubusercontent.com/rollbear/trompeloeil/v39/include/trompeloeil.hpp
    #    URL_HASH            SHA1=5dab5d0b65e6263a96ff4f91a8248ce0959c9d1b
    #    DOWNLOAD_NO_EXTRACT TRUE
    #    DOWNLOAD_DIR        ${CMAKE_CURRENT_BINARY_DIR}/trompeloeil-src/trompeloeil
    #    )

    #FetchContent_GetProperties(trompeloeil)
    #if(NOT trompeloeil_POPULATED)
    #    FetchContent_Populate(trompeloeil)

    #    add_library(trompeloeil INTERFACE IMPORTED GLOBAL)
    #endif()
    #set_property(
    #    TARGET trompeloeil
    #    PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR}/trompeloeil-src/trompeloeil
    #    )
    #set_property(
    #    TARGET trompeloeil
    #    PROPERTY INTERFACE_COMPILE_FEATURES cxx_std_14
    #    )
else()
    find_package(trompeloeil REQUIRED)
endif()


