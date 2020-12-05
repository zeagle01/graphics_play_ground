

cmake_minimum_required(VERSION 3.15)
project(vk_render)


add_executable(vk_render 
main.cpp
Hello_VK_Triangle.cpp
Hello_VK_Triangle.h
)
target_compile_features(vk_render PRIVATE cxx_std_17)

set_property(TARGET vk_render PROPERTY FOLDER examples)



###########glfw
#hunter_add_package(GLFW)
#find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(vk_render PUBLIC glfw)


find_package(glm CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
target_link_libraries(vk_render PRIVATE glm::glm)


###########vulkan
#find_package(Vulkan CONFIG REQUIRED)
##target_compile_definitions(vk_render PRIVATE VK_USE_PLATFORM_WIN32_KHR)
##target_include_directories(vk_render PRIVATE Vulkan::Vulkan)
#target_link_libraries(vk_render Vulkan::Vulkan)

set(vk_render_vulkan_dir "" CACHE PATH "vulkan install dir")

message("vk_render_vulkan_dir: {vk_render_vulkan_dir}")

target_include_directories(vk_render PRIVATE ${vk_render_vulkan_dir}/Include)
target_link_libraries(vk_render PRIVATE ${vk_render_vulkan_dir}/Lib/vulkan-1.lib)


###########glog
find_package(glog CONFIG REQUIRED)
target_link_libraries(vk_render PUBLIC glog::glog)

###########gflags
find_package(gflags CONFIG REQUIRED)
target_link_libraries(vk_render PUBLIC gflags)
