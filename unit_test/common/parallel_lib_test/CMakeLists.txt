

cmake_minimum_required(VERSION 3.20)

project(parallel_lib_test)

add_executable(parallel_lib_test)

target_sources(parallel_lib_test PUBLIC
	FILE_SET cxx_modules
	TYPE CXX_MODULES
	FILES 
	main.cpp
	thread_pool_test.cpp
	for_each_test.cpp
)

set_target_properties(parallel_lib_test PROPERTIES FOLDER unit_test/common)
target_compile_features(parallel_lib_test  PRIVATE cxx_std_20)

target_link_libraries(parallel_lib_test PRIVATE parallel_lib)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(parallel_lib_test  PRIVATE GTest::gmock_main)


