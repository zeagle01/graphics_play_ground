
cmake_minimum_required(VERSION 3.26)

project(quick_shell)


add_library(quick_shell )

set_target_properties(quick_shell PROPERTIES FOLDER common)

target_sources(quick_shell PUBLIC
	FILE_SET cxx_modules
	TYPE CXX_MODULES
	FILES 
	main_window.cpp ## imp
	main_window.ixx  ## main module definition 
	ui_components.ixx  
	ui_event.ixx  
	GLFW_wrapper.cpp
	UI_wrapper.cpp
	render.cpp
	shader.cpp
	vertex_array.cpp
	ui_component_imp.cpp
)

### moduel support end

target_compile_features(quick_shell PRIVATE cxx_std_20)

########## ce_log
target_link_libraries(quick_shell PRIVATE ce_log)

########## clumsy_lib
target_link_libraries(quick_shell PRIVATE clumsy_lib)


########## glfw3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(quick_shell PRIVATE glfw)


########## glad
target_link_libraries(quick_shell PRIVATE glad)


########## imgui
target_link_libraries(quick_shell PRIVATE my_imgui)

########## magic_enum
target_link_libraries(quick_shell PRIVATE my_magic_enum)
