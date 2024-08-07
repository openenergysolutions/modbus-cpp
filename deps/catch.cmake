if(MODBUS_VENDORED_DEPS)
    include(FetchContent)
    Include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.13.10)

    FetchContent_MakeAvailable(Catch2)
else()
    find_package(Catch2 REQUIRED) 
endif()
