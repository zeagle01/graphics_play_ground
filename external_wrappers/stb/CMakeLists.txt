cmake_minimum_required(VERSION 3.15)

project(my_stb)

set(stb_dir ${CMAKE_SOURCE_DIR}/external/stb)

add_library(my_stb 
    ${stb_dir}/stb_image.h
    ${stb_dir}/stb_image.cpp
    )


target_include_directories(my_stb PUBLIC ${stb_dir})

set_property(TARGET my_stb PROPERTY FOLDER external_wrappers)

