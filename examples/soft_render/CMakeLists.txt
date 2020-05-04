



cmake_minimum_required(VERSION 3.12)


project(soft_render)

add_executable(soft_render main.cpp)

target_compile_features(soft_render PRIVATE cxx_std_17)
