

cmake_minimum_required(VERSION 3.22)

project(component_app)

add_executable(component_app  
	main.cpp
)

target_sources(component_app PUBLIC
    FILE_SET CXX_MODULES FILES
	app.ixx
	components.cpp
	plane_maker.cpp
	matrix_def.cpp
)

set_target_properties(component_app PROPERTIES FOLDER examples)

target_compile_features(component_app PRIVATE cxx_std_20)

#link libs
target_link_libraries(component_app PRIVATE quick_shell)

target_link_libraries(component_app PRIVATE sim_lib)

target_link_libraries(component_app PRIVATE geometry_lib)

target_link_libraries(component_app PRIVATE matrix_math)

target_link_libraries(component_app PRIVATE clumsy_lib)
