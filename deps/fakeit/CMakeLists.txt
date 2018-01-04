cmake_minimum_required (VERSION 3.8)

project(fakeit)

add_library(fakeit INTERFACE)
target_include_directories(fakeit INTERFACE ./fakeit/single_header/catch)
target_compile_features(fakeit INTERFACE cxx_std_11)
