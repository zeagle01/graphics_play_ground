

cmake_minimum_required(VERSION 3.22)

project(graphic_app_0)

add_executable(graphic_app_0  
	main.cpp
)

set_target_properties(graphic_app_0 PROPERTIES FOLDER examples)

target_compile_features(graphic_app_0 PRIVATE cxx_std_20)


target_link_libraries(graphic_app_0 PRIVATE quick_shell)
