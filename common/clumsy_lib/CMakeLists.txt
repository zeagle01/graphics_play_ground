
cmake_minimum_required(VERSION 3.15)

project(clumsy_lib  )


add_library(clumsy_lib STATIC 
include/clumsy_lib/type_list.h
include/clumsy_lib/macro_loop.h
include/clumsy_lib/class_reflection.h
include/clumsy_lib/type_map.h
include/clumsy_lib/variable_set.h
include/clumsy_lib/morphism_types.h
include/clumsy_lib/variable_accecor.h
include/clumsy_lib/dependent_variable_set.h
include/clumsy_lib/static_loop.h
clumsy_lib.cpp
)

target_compile_features(clumsy_lib PRIVATE cxx_std_20)



target_include_directories(clumsy_lib
PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
PUBLIC
$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(clumsy_lib PROPERTIES FOLDER common)
