
cmake_minimum_required(VERSION 3.12)

project(gl_render)

add_executable(gl_render 
main.cpp

renderer.cpp
renderer.h
vertex_buffer.h
vertex_buffer.cpp
index_buffer.h
index_buffer.cpp
vertex_array.h
vertex_array.cpp
vertex_buffer_layout.h
vertex_buffer_layout.cpp
shader.h
shader.cpp
texture.h
texture.cpp

#${CMAKE_SOURCE_DIR}/external/stb/stb_image.h
${CMAKE_SOURCE_DIR}/external/stb/stb_image.cpp
)

target_compile_features(gl_render PRIVATE cxx_std_20)


target_link_libraries(gl_render PRIVATE glad)


target_include_directories(gl_render PRIVATE ${CMAKE_SOURCE_DIR}/external/stb)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(gl_render PUBLIC glfw)


###########glog
find_package(glog CONFIG REQUIRED)
target_link_libraries(gl_render PUBLIC glog::glog)

###########gflags
find_package(gflags CONFIG REQUIRED)
target_link_libraries(gl_render PUBLIC gflags)


set_property(TARGET gl_render PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:gl_render>)

add_custom_command(TARGET gl_render
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../cases $<TARGET_FILE_DIR:gl_render>/cases
    COMMENT "copy cases" VERBATIM
    )

add_custom_command(TARGET gl_render
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:gl_render>/resources
    COMMENT "copy resources" VERBATIM
    )

