cmake_minimum_required(VERSION 3.15)

project(my_magic_enum)

set(magic_enum_dir ${CMAKE_SOURCE_DIR}/external/magic_enum)

add_library(my_magic_enum 
	dummy.cpp
)

target_include_directories(my_magic_enum PUBLIC ${magic_enum_dir}/include)

target_compile_features(my_magic_enum  PRIVATE cxx_std_20)

set_property(TARGET my_magic_enum PROPERTY FOLDER external_wrappers)

