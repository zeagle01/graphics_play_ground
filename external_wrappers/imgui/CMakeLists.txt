

cmake_minimum_required(VERSION 3.15)



project(my_imgui)



set(imgui_dir ${CMAKE_SOURCE_DIR}/external/imgui)

add_library(my_imgui 
    ${imgui_dir}/imconfig.h
    ${imgui_dir}/imgui.cpp
    ${imgui_dir}/imgui.h
    ${imgui_dir}/imgui_demo.cpp
    ${imgui_dir}/imgui_draw.cpp
    ${imgui_dir}/imgui_internal.h
    ${imgui_dir}/imgui_widgets.cpp
    ${imgui_dir}/imstb_rectpack.h
    ${imgui_dir}/imstb_textedit.h
    ${imgui_dir}/imstb_truetype.h

    ${imgui_dir}/examples/imgui_impl_glfw.h
    ${imgui_dir}/examples/imgui_impl_glfw.cpp

    ${imgui_dir}/examples/imgui_impl_opengl3.h
    ${imgui_dir}/examples/imgui_impl_opengl3.cpp
    )


target_include_directories(my_imgui PUBLIC ${imgui_dir})
target_include_directories(my_imgui PUBLIC ${imgui_dir}/examples)

set_property(TARGET my_imgui PROPERTY FOLDER external_wrappers)

target_compile_definitions(my_imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)



#find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(my_imgui PUBLIC glfw)

target_link_libraries(my_imgui PUBLIC glad)

