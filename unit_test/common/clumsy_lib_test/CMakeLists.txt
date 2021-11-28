
cmake_minimum_required(VERSION 3.15)

project(clumsy_lib_test)



add_executable(clumsy_lib_test  
main.cpp
type_list_test.cpp
macro_loop_test.cpp
type_map_test.cpp
copy_struct_test.cpp
class_reflection_test.cpp
)


target_compile_features(clumsy_lib_test PRIVATE cxx_std_20)

target_link_libraries(clumsy_lib_test PRIVATE clumsy_lib)

set_target_properties(clumsy_lib_test PROPERTIES FOLDER unit_test/common)


include(${CMAKE_SOURCE_DIR}/cmake/gtest.cmake)
link_to_gtest(clumsy_lib_test ${CMAKE_SOURCE_DIR}/external/gtest)
