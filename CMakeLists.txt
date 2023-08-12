cmake_minimum_required(VERSION 3.15)


set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "")
message( "CMAKE_TOOLCHAIN_FILE is ${CMAKE_TOOLCHAIN_FILE}")

project(graphics_play_ground)


include(GNUInstallDirs)
#set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/shellet_install)
#set(CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})

add_subdirectory(examples)

add_subdirectory(external_wrappers)

add_subdirectory(common/matrix_math)
add_subdirectory(common/clumsy_lib)

add_subdirectory(unit_test/common/matrix_math_test)
add_subdirectory(unit_test/common/clumsy_lib_test)

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
