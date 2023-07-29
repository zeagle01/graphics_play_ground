

cmake_minimum_required(VERSION 3.15)

project(matrix_math_test)



add_executable(matrix_math_test  
	main.cpp
	matrix_math_test.cpp
	linear_system_solver_test.cpp
)


target_link_libraries(matrix_math_test PRIVATE matrix_math)

set_target_properties(matrix_math_test PROPERTIES FOLDER unit_test/common)


find_package(GTest CONFIG REQUIRED)
target_link_libraries(matrix_math_test PRIVATE GTest::gmock_main)

target_compile_features(matrix_math_test PRIVATE cxx_std_20)

