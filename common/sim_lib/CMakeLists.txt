
cmake_minimum_required(VERSION 3.26)

project(sim_lib)

set(partition_files
	simulator_imp.cpp
	solver.cpp
	solver_data_update.cpp
	solver_dummy.cpp
)

add_library(sim_lib  
	${partition_files}
	simulator.cpp
)

set_target_properties(sim_lib PROPERTIES FOLDER common)


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

target_sources(sim_lib PUBLIC
	FILE_SET cxx_modules
	TYPE CXX_MODULES
	FILES 
	simulator.ixx 
	sim_data.ixx 
)

set_source_files_properties(${partition_files} PROPERTIES COMPILE_FLAGS "/internalPartition")

### moduel support end

target_compile_features(sim_lib PRIVATE cxx_std_20)

########## ce_log
target_link_libraries(sim_lib PRIVATE ce_log)

########## clumsy_lib
target_link_libraries(sim_lib PRIVATE clumsy_lib)

########## matrix_math
target_link_libraries(sim_lib PRIVATE matrix_math)

