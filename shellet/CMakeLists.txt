




add_library(shellet 
    src/Mesh_Loader.cpp
    src/Shader.cpp
    src/Scene.cpp
    #src/Plot_Data_IO.cpp
    )
add_executable(shellet_test 
    test/test_main.cpp
    test/Mesh_Loader_Test.cpp
    test/Shader_Test.cpp
    test/Scene_Test.cpp
    test/Plot_Data_IO_Test.cpp
    src/Scene.cpp#for glad load  reason
    )


target_include_directories(shellet PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/shellet>
    )
target_compile_features(shellet PRIVATE cxx_std_11)
target_compile_features(shellet_test PRIVATE cxx_std_11)
#target_compile_options(shellet PRIVATE "-std=c++17")
#target_compile_options(shellet_test PRIVATE "-std=c++17")

add_custom_command(TARGET shellet_test
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/test/test_data $<TARGET_FILE_DIR:shellet_test>/test_data
    COMMENT "copy test_data" VERBATIM
    )

add_custom_command(TARGET shellet
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders $<TARGET_FILE_DIR:shellet>/shaders
    COMMENT "copy shaders" VERBATIM
    )

target_link_libraries(shellet_test PRIVATE shellet)



hunter_add_package(jsoncpp)
find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(shellet PRIVATE jsoncpp_lib_static)

hunter_add_package(Boost COMPONENTS system filesystem)
find_package(Boost CONFIG REQUIRED system filesystem)
target_link_libraries(shellet PRIVATE  Boost::filesystem Boost::system)
target_link_libraries(shellet_test PRIVATE  Boost::filesystem Boost::system)


#find_package(glad CONFIG REQUIRED)
target_link_libraries(shellet PUBLIC  glad)

hunter_add_package(GLFW)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(shellet PUBLIC  glfw)


hunter_add_package(Glog)
find_package(glog CONFIG REQUIRED)
target_link_libraries(shellet PUBLIC glog::glog)

hunter_add_package(gflags)
find_package(gflags CONFIG REQUIRED)
target_link_libraries(shellet PUBLIC gflags)



hunter_add_package(GTest)
find_package(GMock CONFIG REQUIRED)
target_link_libraries(shellet_test PRIVATE GMock::gmock)



