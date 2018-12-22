cmake_minimum_required (VERSION 3.8)

project(trompeloeil)

add_library(trompeloeil INTERFACE)
target_include_directories(trompeloeil INTERFACE ./trompeloeil/include)
target_compile_features(trompeloeil INTERFACE cxx_std_14)
