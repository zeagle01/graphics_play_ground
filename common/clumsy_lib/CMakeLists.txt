
cmake_minimum_required(VERSION 3.15)

project(clumsy_lib  )

set(partition_files "")

add_library(clumsy_lib STATIC 
include/clumsy_lib/type_list.h
include/clumsy_lib/type_list_helper.h
include/clumsy_lib/macro_loop.h
include/clumsy_lib/class_reflection.h
include/clumsy_lib/type_map.h
include/clumsy_lib/variable_set.h
include/clumsy_lib/morphism_types.h
include/clumsy_lib/variable_accecor.h
include/clumsy_lib/static_loop.h
include/clumsy_lib/raw_pointer.h
include/clumsy_lib/static_type_map.h
include/clumsy_lib/dynamic_type_map.h
include/clumsy_lib/field_defines.h
include/clumsy_lib/morphysm.h
include/clumsy_lib/type_getter.h
include/clumsy_lib/weak_ptr.h
clumsy_lib.cpp
${partition_files}
)

target_compile_features(clumsy_lib PRIVATE cxx_std_20)



target_include_directories(clumsy_lib
PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
PUBLIC
$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(clumsy_lib PROPERTIES FOLDER common)




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

target_sources(clumsy_lib PUBLIC
	FILE_SET cxx_modules
	TYPE CXX_MODULES
	FILES 
	clumsy_lib.ixx
	literal_value.ixx
	static_dep_graph.ixx
)

set_source_files_properties(${partition_files} PROPERTIES COMPILE_FLAGS "/internalPartition")

### moduel support end
