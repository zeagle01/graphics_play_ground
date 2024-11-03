cmake_minimum_required(VERSION 3.20)

project(rigid_body)

add_executable(rigid_body )

target_sources(rigid_body PUBLIC
    FILE_SET CXX_MODULES FILES
	main.cpp
	rigid_body_dynamic.ixx
)

set_target_properties(rigid_body PROPERTIES FOLDER examples)

target_compile_features(rigid_body PRIVATE cxx_std_20)

target_link_libraries(rigid_body PRIVATE quick_shell)
target_link_libraries(rigid_body PRIVATE matrix_math)
