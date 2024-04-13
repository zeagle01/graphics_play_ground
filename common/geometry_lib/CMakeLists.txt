
cmake_minimum_required(VERSION 3.26)

project(geometry_lib)


add_library(geometry_lib )

set_target_properties(geometry_lib PROPERTIES FOLDER common)


target_sources(geometry_lib PUBLIC
	FILE_SET cxx_modules
	TYPE CXX_MODULES
	FILES 
	geometry_lib.ixx  ## main module definition
	bvh.ixx  
	aabb.ixx  
	bvh.cpp
)

### moduel support end
target_compile_features(geometry_lib PRIVATE cxx_std_20)

target_link_libraries(geometry_lib PRIVATE matrix_math)


