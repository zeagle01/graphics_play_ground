

cmake_minimum_required(VERSION 3.29)

project(parallel_lib)

add_library(parallel_lib)

target_sources(parallel_lib PUBLIC
	FILE_SET cxx_modules
	TYPE CXX_MODULES
	FILES 
	parallel.ixx  ## main module definition
	parallel.cpp  ## imp unit  
	##interface partition

	##internal partition
	thread_pool.cpp  
)

set_target_properties(parallel_lib PROPERTIES FOLDER common)

target_compile_features(parallel_lib PRIVATE cxx_std_20)
