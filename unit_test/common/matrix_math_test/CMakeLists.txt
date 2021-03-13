

cmake_minimum_required(VERSION 3.15)

project(matrix_math_test)



add_executable(matrix_math_test  
main.cpp
matrix_math_test.cpp
)


target_link_libraries(matrix_math_test PRIVATE matrix_math)

set_target_properties(matrix_math_test PROPERTIES FOLDER unit_test/common)


include(${CMAKE_SOURCE_DIR}/cmake/gtest.cmake)
link_to_gtest(matrix_math_test ${CMAKE_SOURCE_DIR}/external/gtest)

