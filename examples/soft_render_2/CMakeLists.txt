cmake_minimum_required(VERSION 3.15)

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
)

##########imgui
target_link_libraries(soft_render_2 PRIVATE my_imgui)


##########glfw3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(soft_render_2 PUBLIC glfw)

##########glad
target_link_libraries(soft_render_2 PRIVATE glad)

set_property(TARGET soft_render_2 PROPERTY FOLDER examples)

