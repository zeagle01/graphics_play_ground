
cmake_minimum_required(VERSION 3.15)

project(clumsy_editor)


add_executable(clumsy_editor 
	editor_app.cpp
	editor_layer.h
	editor_layer.cpp
	#shader_sources.h
)

set_target_properties(clumsy_editor PROPERTIES FOLDER examples/clumsy_engine)

target_compile_features(clumsy_editor PRIVATE cxx_std_20)

set_target_properties(clumsy_editor
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/bin
)

target_link_libraries(clumsy_editor PUBLIC clumsy_engine)

