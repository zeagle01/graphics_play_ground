




add_library(shellet 
    src/Mesh_Loader.cpp
    src/Shader.cpp
    src/Scene.cpp
    src/Scene.h
	src/Simulator.cpp
	src/Simulator.h
	src/Simulation_Data.cpp
	src/Simulation_Data.h
	src/PD_Simulator.cpp
	src/PD_Simulator.h
	src/FEM_Simulator.cpp
	src/FEM_Simulator.h
	src/predef_types.h
	src/Topology_Computer.h
	src/Topology_Computer.cpp
    #src/Plot_Data_IO.cpp
    )
add_executable(shellet_test 
    test/test_main.cpp
    test/Mesh_Loader_Test.cpp
    test/Mesh_Loader_Test.h
    test/Shader_Test.cpp
    test/Shader_Test.h
    test/Scene_Test.cpp
    test/Scene_Test.h
    test/Plot_Data_IO_Test.cpp
    src/Scene.cpp#for glad load  reason
	test/Topology_Computer_Test.h
	test/Topology_Computer_Test.cpp
	test/Simulator_Test.cpp
	test/FEM_Simulator_Test.cpp
    )

set_property(TARGET shellet_test PROPERTY FOLDER shellet)
set_property(TARGET shellet PROPERTY FOLDER shellet)

set_property(TARGET shellet_test PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:shellet_test>)
set_property(TARGET shellet PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:shellet>)

target_include_directories(shellet PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/shellet>
    )
target_compile_features(shellet PRIVATE cxx_std_17)
target_compile_features(shellet_test PRIVATE cxx_std_17)
#target_compile_options(shellet PRIVATE "-std=c++17")
#target_compile_options(shellet_test PRIVATE "-std=c++17")

#add_custom_command(TARGET shellet_test
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/test/test_data $<TARGET_FILE_DIR:shellet_test>/test_data
#    COMMENT "copy test_data" VERBATIM
#    )

#add_custom_command(TARGET shellet
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders $<TARGET_FILE_DIR:shellet>/shaders
#    COMMENT "copy shaders" VERBATIM
#    )

target_link_libraries(shellet_test PRIVATE shellet)



find_package(jsoncpp CONFIG REQUIRED PATHS ${CMAKE_SOURCE_DIR}/Install/lib/cmake/jsoncpp)
target_link_libraries(shellet PRIVATE jsoncpp_lib_static)


target_link_libraries(shellet PUBLIC  glad)

target_link_libraries(shellet PUBLIC  glfw)


find_package(glog CONFIG REQUIRED PATHS ${CMAKE_SOURCE_DIR}/Install/lib/cmake/glog)
target_link_libraries(shellet PUBLIC glog::glog)

find_package(gflags CONFIG REQUIRED PATHS ${CMAKE_SOURCE_DIR}/Install/lib/cmake/gflags)
target_link_libraries(shellet PUBLIC gflags)



find_package(GMock CONFIG REQUIRED PATHS ${CMAKE_SOURCE_DIR}/Install/lib/cmake/GMock)
target_link_libraries(shellet_test PRIVATE GMock::gmock)


find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(shellet PUBLIC Eigen3::Eigen)
