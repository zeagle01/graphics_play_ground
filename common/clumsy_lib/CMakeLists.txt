
cmake_minimum_required(VERSION 3.15)

project(clumsy_lib  )


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
	include/clumsy_lib/enum_tagged_morphysm.h
	clumsy_lib.cpp
)

target_compile_features(clumsy_lib PRIVATE cxx_std_20)


target_include_directories(clumsy_lib
PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
PUBLIC
$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(clumsy_lib PROPERTIES FOLDER common)


target_sources(clumsy_lib PUBLIC
	FILE_SET cxx_modules
	TYPE CXX_MODULES
	FILES 
	clumsy_lib.ixx
	literal_value.ixx
	static_dep_graph.ixx
	dispatchor.ixx
	enum_tagged_morphysm.ixx
)


target_link_libraries(clumsy_lib PRIVATE my_magic_enum)
