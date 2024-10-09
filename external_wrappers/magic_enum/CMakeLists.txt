cmake_minimum_required(VERSION 3.15)

project(my_magic_enum)

set(magic_enum_dir ${CMAKE_SOURCE_DIR}/external/magic_enum)

add_library(my_magic_enum INTERFACE)

target_include_directories(my_magic_enum INTERFACE ${magic_enum_dir}/include)

set_property(TARGET my_magic_enum PROPERTY FOLDER external_wrappers)

