cmake_minimum_required(VERSION 3.21)

project(soft_render_2)

add_executable(
	soft_render_2
	main.cpp
	OpenGL_Wrapper.h
	OpenGL_Wrapper.cpp
	GLFW_Wrapper.h
	GLFW_Wrapper.cpp
	Drawing_Buffer.h
	Drawing_Buffer.cpp
	Imgui_Wrapper.h
	Imgui_Wrapper.cpp
	Spinning_Cube.h
	Spinning_Cube.cpp
	Camara.h
	Camara.cpp
	mat.h
	type_list.h
	type_map.h
	macro_loop.h
	member_extractor.h
	ui_data_mapper.h
	Spinning_Cube_App.h
	Spinning_Cube_App.cpp
	serialization.h
)

target_compile_features(soft_render_2 PRIVATE cxx_std_20)

##########imgui
target_link_libraries(soft_render_2 PRIVATE my_imgui)

##########glfw3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(soft_render_2 PUBLIC glfw)

##########glad
target_link_libraries(soft_render_2 PRIVATE glad)

set_property(TARGET soft_render_2 PROPERTY FOLDER examples)

