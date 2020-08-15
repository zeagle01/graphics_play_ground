


cmake_minimum_required(VERSION 3.15)


project(soft_render_2)


add_executable(
	soft_render_2_test
	soft_render.h
	test_main.cpp
	soft_render_test.cpp
)


set_property(TARGET soft_render_2_test PROPERTY FOLDER examples)




hunter_add_package(GTest)
find_package(GMock CONFIG REQUIRED)
target_link_libraries(soft_render_2_test PRIVATE GMock::gmock)
