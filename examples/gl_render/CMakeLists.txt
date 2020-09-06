
cmake_minimum_required(VERSION 3.15)
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
test_texture2d.cpp
test_texture2d.h
test_batch_rendering.cpp
test_batch_rendering.h

#${CMAKE_SOURCE_DIR}/external/stb/stb_image.h
${CMAKE_SOURCE_DIR}/external/stb/stb_image.cpp
)


set_property(TARGET gl_render  PROPERTY FOLDER examples)


target_compile_features(gl_render PRIVATE cxx_std_20)


target_link_libraries(gl_render PRIVATE glad)



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

##########imgui
target_link_libraries(gl_render PRIVATE my_imgui)

#########glm
FetchContent_Declare(
  glm_repo
  GIT_REPOSITORY ${CMAKE_SOURCE_DIR}/external/glm
  GIT_TAG        master
)
FetchContent_MakeAvailable(glm_repo)
target_link_libraries(gl_render PRIVATE glm)






set_property(TARGET gl_render PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:gl_render>)

#add_custom_command(TARGET gl_render
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../cases $<TARGET_FILE_DIR:gl_render>/cases
#    COMMENT "copy cases" VERBATIM
#    )

#add_custom_command(TARGET gl_render
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:gl_render>/resources
#    COMMENT "copy resources" VERBATIM
#    )
#
