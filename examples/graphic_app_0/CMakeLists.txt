

cmake_minimum_required(VERSION 3.22)

project(graphic_app_0)

add_executable(graphic_app_0  
	main.cpp
	app.h
	app.cpp
	mouse_picker.h
	mouse_picker.cpp
)

target_sources(graphic_app_0 PUBLIC
    FILE_SET CXX_MODULES FILES
	app.ixx
	plane_maker.ixx
)

set_target_properties(graphic_app_0 PROPERTIES FOLDER examples)

target_compile_features(graphic_app_0 PRIVATE cxx_std_20)

target_link_libraries(graphic_app_0 PRIVATE quick_shell)

target_link_libraries(graphic_app_0 PRIVATE sim_lib)

target_link_libraries(graphic_app_0 PRIVATE geometry_lib)

target_link_libraries(graphic_app_0 PRIVATE matrix_math)
