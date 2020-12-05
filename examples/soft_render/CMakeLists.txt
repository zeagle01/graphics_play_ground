



cmake_minimum_required(VERSION 3.15)

project(soft_render)

add_executable(soft_render 
	main.cpp
	soft_render.h
	frame_buffer.h
	out_device.h
	rasterizer.h
	math.h

	soft_render_test.cpp
)

target_compile_features(soft_render PRIVATE cxx_std_17)

target_link_libraries(soft_render PRIVATE shellet)

set_property(TARGET soft_render PROPERTY FOLDER examples)

set_property(TARGET soft_render PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:soft_render>)


#hunter_add_package(GTest)
find_package(GMock CONFIG REQUIRED PATHS ${CMAKE_SOURCE_DIR}/Install/lib/cmake/GMock)
target_link_libraries(soft_render PRIVATE GMock::gmock)


#find_package(GTest CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
#target_link_libraries(soft_render PRIVATE GTest::gmock)


#include(${CMAKE_SOURCE_DIR}/cmake/gtest.cmake)
#link_to_gtest(soft_render ${CMAKE_SOURCE_DIR}/external/gtest)

