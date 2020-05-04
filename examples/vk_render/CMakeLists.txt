

cmake_minimum_required(VERSION 3.12)

project(vk_render)


add_executable(vk_render 
main.cpp
Hello_VK_Triangle.cpp
Hello_VK_Triangle.h
)
target_compile_features(vk_render PRIVATE cxx_std_17)



###########glfw
#hunter_add_package(GLFW)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(vk_render PUBLIC glfw)


include(FetchContent)
###########glm
FetchContent_Declare(
  glm_repo
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(glm_repo)
target_link_libraries(vk_render PRIVATE glm)


###########vulkan
#find_package(Vulkan CONFIG REQUIRED)
##target_compile_definitions(vk_render PRIVATE VK_USE_PLATFORM_WIN32_KHR)
##target_include_directories(vk_render PRIVATE Vulkan::Vulkan)
#target_link_libraries(vk_render Vulkan::Vulkan)

set(vk_render_vulkan_dir "" CACHE PATH "vulkan install dir")
target_include_directories(vk_render PRIVATE ${vk_render_vulkan_dir}/Include)
target_link_libraries(vk_render PRIVATE ${vk_render_vulkan_dir}/Lib/vulkan-1.lib)


###########glog
find_package(glog CONFIG REQUIRED)
target_link_libraries(vk_render PUBLIC glog::glog)

###########gflags
find_package(gflags CONFIG REQUIRED)
target_link_libraries(vk_render PUBLIC gflags)
