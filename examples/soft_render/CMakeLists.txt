



cmake_minimum_required(VERSION 3.12)


project(soft_render)

add_executable(soft_render main.cpp)

target_compile_features(soft_render PRIVATE cxx_std_17)

target_link_libraries(soft_render PRIVATE shellet)

set_property(TARGET soft_render PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:soft_render>)

add_custom_command(TARGET soft_render
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../cases $<TARGET_FILE_DIR:soft_render>/cases
    COMMENT "copy cases" VERBATIM
    )
