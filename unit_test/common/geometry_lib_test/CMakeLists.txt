

cmake_minimum_required(VERSION 3.20)

project(geometry_lib_test)

add_executable(geometry_lib_test
	main.cpp
	bvh_test.cpp
	aabb_test.cpp
)


target_link_libraries(geometry_lib_test PRIVATE geometry_lib)

set_target_properties(geometry_lib_test PROPERTIES FOLDER unit_test/common)


find_package(GTest CONFIG REQUIRED)
target_link_libraries(geometry_lib_test  PRIVATE GTest::gmock_main)

target_link_libraries(geometry_lib_test  PRIVATE matrix_math)

target_compile_features(geometry_lib_test  PRIVATE cxx_std_20)

