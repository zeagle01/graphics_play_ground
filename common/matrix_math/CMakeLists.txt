

cmake_minimum_required(VERSION 3.26)

project(matrix_math  )

add_library(matrix_math STATIC 
	include/matrix_math/matrix_math.h
	matrix_math.cpp
	include/matrix_math/linear_system_solver.h
)

target_include_directories(matrix_math
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PUBLIC
	$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(matrix_math PROPERTIES FOLDER common)

target_sources(matrix_math PUBLIC
	FILE_SET cxx_modules
	TYPE CXX_MODULES
	FILES 
	sparse_matrix.ixx
	matrix.ixx
)


### moduel support end

target_compile_features(matrix_math PRIVATE cxx_std_20)
