cmake_minimum_required (VERSION 3.8)

project(asio)

add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ./asio/asio/include)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)
target_compile_features(asio INTERFACE cxx_std_11)

if(WIN32)
    target_link_libraries(asio INTERFACE ws2_32 wsock32) # Link to Winsock
    target_compile_definitions(asio INTERFACE _WIN32_WINNT=0x0601) # Windows 7 and up
endif()
