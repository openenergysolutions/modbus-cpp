# A macro for find_package that also checks the dependencies
# in sub-directories.
# Code highly inspired from Daniel Pfeifer "Effective CMake" talk at C++Now 2017
# available here: https://youtu.be/bsXLMQ6WgIk

macro(find_package)
    if(NOT "${ARG0}" IN_LIST as_subproject)
        _find_package(${ARGV})
    endif()
endmacro()
