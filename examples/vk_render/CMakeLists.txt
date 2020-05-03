

cmake_minimum_required(VERSION 3.12)

project(vk_render)


add_executable(vk_render main.cpp)


target_link_libraries(shellet PRIVATE glfw)