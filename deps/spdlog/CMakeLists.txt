cmake_minimum_required (VERSION 3.8)

project(spdlog)

add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE ./spdlog/include)
target_compile_features(spdlog INTERFACE cxx_std_11)
