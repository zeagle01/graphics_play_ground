cmake_minimum_required(VERSION 3.15)

project(soft_render_2)

add_executable(
	soft_render_2
	main.cpp
)

##########imgui
target_link_libraries(soft_render_2 PRIVATE my_imgui)


##########glfw3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(soft_render_2 PUBLIC glfw)

##########glad
target_link_libraries(soft_render_2 PRIVATE glad)

set_property(TARGET soft_render_2 PROPERTY FOLDER examples)

