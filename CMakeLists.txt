cmake_minimum_required(VERSION 3.12)

include(cmake/HunterGate.cmake)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.18.12.tar.gz"
    SHA1 "7e8c74787e08c476484d3e5106524fe7a5a0cc56"
)

project(graphics_play_ground)

include(GNUInstallDirs)
#set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/shellet_install)
#set(CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})

add_subdirectory(shellet)
add_subdirectory(examples)
add_subdirectory(glad)
