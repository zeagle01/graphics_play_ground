
cmake_minimum_required(VERSION 3.12)

project(gl_render)

add_executable(gl_render main.cpp)

target_compile_features(gl_render PRIVATE cxx_std_20)


target_link_libraries(gl_render PRIVATE glad)


find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(gl_render PUBLIC glfw)


###########glog
find_package(glog CONFIG REQUIRED)
target_link_libraries(gl_render PUBLIC glog::glog)

###########gflags
find_package(gflags CONFIG REQUIRED)
target_link_libraries(gl_render PUBLIC gflags)


