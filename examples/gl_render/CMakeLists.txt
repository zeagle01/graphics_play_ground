
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

test.cpp
test.h
test_clear_color.cpp
test_clear_color.h

#${CMAKE_SOURCE_DIR}/external/stb/stb_image.h
${CMAKE_SOURCE_DIR}/external/stb/stb_image.cpp

#imgui
${CMAKE_SOURCE_DIR}/external/imgui-master/imconfig.h
${CMAKE_SOURCE_DIR}/external/imgui-master/imgui.cpp
${CMAKE_SOURCE_DIR}/external/imgui-master/imgui.h
${CMAKE_SOURCE_DIR}/external/imgui-master/imgui_demo.cpp
${CMAKE_SOURCE_DIR}/external/imgui-master/imgui_draw.cpp
${CMAKE_SOURCE_DIR}/external/imgui-master/imgui_internal.h
${CMAKE_SOURCE_DIR}/external/imgui-master/imgui_widgets.cpp
${CMAKE_SOURCE_DIR}/external/imgui-master/imstb_rectpack.h
${CMAKE_SOURCE_DIR}/external/imgui-master/imstb_textedit.h
${CMAKE_SOURCE_DIR}/external/imgui-master/imstb_truetype.h

${CMAKE_SOURCE_DIR}/external/imgui-master/examples/imgui_impl_glfw.h
${CMAKE_SOURCE_DIR}/external/imgui-master/examples/imgui_impl_glfw.cpp

${CMAKE_SOURCE_DIR}/external/imgui-master/examples/imgui_impl_opengl3.h
${CMAKE_SOURCE_DIR}/external/imgui-master/examples/imgui_impl_opengl3.cpp
)

target_compile_features(gl_render PRIVATE cxx_std_20)


target_link_libraries(gl_render PRIVATE glad)



find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(gl_render PUBLIC glfw)


###########glog
find_package(glog CONFIG REQUIRED)
target_link_libraries(gl_render PUBLIC glog::glog)

###########stb
target_include_directories(gl_render PRIVATE ${CMAKE_SOURCE_DIR}/external/stb)

###########gflags
find_package(gflags CONFIG REQUIRED)
target_link_libraries(gl_render PUBLIC gflags)

include(FetchContent)

#########imgui
target_include_directories(gl_render PRIVATE ${CMAKE_SOURCE_DIR}/external/imgui-master)
target_include_directories(gl_render PRIVATE ${CMAKE_SOURCE_DIR}/external/imgui-master/examples)

#########glm
FetchContent_Declare(
  glm_repo
  GIT_REPOSITORY ${CMAKE_SOURCE_DIR}/external/glm
  GIT_TAG        master
)
FetchContent_MakeAvailable(glm_repo)
target_link_libraries(gl_render PRIVATE glm)






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

