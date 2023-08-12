



cmake_minimum_required(VERSION 3.15)

project(show_obj)

add_executable(show_obj main.cpp)

target_compile_features(show_obj PRIVATE cxx_std_17)

set_property(TARGET show_obj PROPERTY FOLDER examples/shellet)
set_property(TARGET show_obj PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:show_obj>)

target_link_libraries(show_obj PRIVATE shellet)

#hunter_add_package(Boost COMPONENTS system filesystem)
#find_package(Boost CONFIG REQUIRED system filesystem)
#target_link_libraries(show_obj PRIVATE  Boost::filesystem Boost::system)



#add_custom_command(TARGET show_obj
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/cases $<TARGET_FILE_DIR:show_obj>/cases
#    COMMENT "copy cases" VERBATIM
#    )


#add_custom_command(TARGET show_obj
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shellet/src/shaders $<TARGET_FILE_DIR:show_obj>/shaders
#    COMMENT "copy shaders to main" VERBATIM
#    )



