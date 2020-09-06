cmake_minimum_required(VERSION 3.15)
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
add_subdirectory(clumsy_engine)

add_subdirectory(external_wrappers/imgui)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#set(cmake_version "3.15")
#message("${cmake_version}")


add_executable(install_resources
install_resources.cpp
)

add_custom_command(TARGET install_resources
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
    COMMENT "copy resouces to binary root" VERBATIM
    )

add_custom_command(TARGET install_resources
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/app_projects ${CMAKE_BINARY_DIR}/app_projects
    COMMENT "copy app_projects to binary root" VERBATIM
    )
