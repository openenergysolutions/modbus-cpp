if(MODBUS_VENDORED_DEPS)
    include(FetchContent)

    FetchContent_Declare(
        trompeloeil
        URL                 https://raw.githubusercontent.com/rollbear/trompeloeil/v35/include/trompeloeil.hpp
        URL_HASH            SHA1=6648C6B6B631491B1D04506717401B5A9720D01C
        DOWNLOAD_NO_EXTRACT TRUE
        DOWNLOAD_DIR        ${CMAKE_CURRENT_BINARY_DIR}/trompeloeil-src/trompeloeil
        )

    FetchContent_GetProperties(trompeloeil)
    if(NOT trompeloeil_POPULATED)
        FetchContent_Populate(trompeloeil)

        add_library(trompeloeil INTERFACE IMPORTED GLOBAL)
    endif()
    set_property(
        TARGET trompeloeil
        PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR}/trompeloeil-src
        )
    set_property(
        TARGET trompeloeil
        PROPERTY INTERFACE_COMPILE_FEATURES cxx_std_14
        )
else()
    find_package(trompeloeil REQUIRED)
endif()


