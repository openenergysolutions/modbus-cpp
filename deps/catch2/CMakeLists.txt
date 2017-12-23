cmake_minimum_required (VERSION 3.8)

project(catch2)

add_library(catch2 INTERFACE)
target_include_directories(catch2 INTERFACE ./catch2/single_include)
target_compile_features(catch2 INTERFACE cxx_std_11)
