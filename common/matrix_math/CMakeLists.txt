

cmake_minimum_required(VERSION 3.26)

project(matrix_math  )


set(partition_files "")

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



###moduel support
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)

# UUID at https://gitlab.kitware.com/cmake/cmake/-/blob/master/Help/dev/experimental.rst
if(CMAKE_VERSION VERSION_LESS 3.26)
  # 3.25.2...3.26
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "3c375311-a3c9-4396-a187-3227ef642046")
elseif(CMAKE_VERSION VERSION_LESS 3.27)
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
elseif(CMAKE_VERSION VERSION_LESS 3.28)
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
elseif(CMAKE_VERSION VERSION_LESS 3.29)
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "a816ed09-43d1-40e5-bc8c-1a2824ee194e")
endif()

target_sources(matrix_math PUBLIC
	FILE_SET cxx_modules
	TYPE CXX_MODULES
	FILES 
	sparse_matrix.ixx
)

set_source_files_properties(${partition_files} PROPERTIES COMPILE_FLAGS "/internalPartition")

### moduel support end

target_compile_features(matrix_math PRIVATE cxx_std_20)
