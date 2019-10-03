cmake_minimum_required(VERSION 3.12)

include(cmake/HunterGate.cmake)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.214.tar.gz"
    SHA1 "e14bc153a7f16d6a5eeec845fb0283c8fad8c358"
)

project(graphics_play_ground)

include(GNUInstallDirs)
#set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/shellet_install)
#set(CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})

add_subdirectory(shellet)
add_subdirectory(examples)
add_subdirectory(glad)
