



cmake_minimum_required(VERSION 3.12)


project(show_obj)

add_executable(show_obj main.cpp)

target_link_libraries(show_obj PRIVATE shellet)


add_custom_command(TARGET show_obj
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/cases $<TARGET_FILE_DIR:show_obj>/cases
    COMMENT "copy cases" VERBATIM
    )


add_custom_command(TARGET show_obj
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shellet/src/shaders $<TARGET_FILE_DIR:show_obj>/shaders
    COMMENT "copy shaders to main" VERBATIM
    )
