
cmake_minimum_required(VERSION 3.15)


project(clumsy_engine)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "export all symbols")

add_library(clumsy_engine SHARED
include/clumsy_engine/test.h
src/test.cpp
)


target_include_directories(clumsy_engine
PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:include>
PRIVATE
${CMAKE_CURRENT_BINARY_DIR}
src
)

include(GNUInstallDirs)
install(
TARGETS clumsy_engine
EXPORT clumsy_engine_targets
LIBRARY DESTINATION ${CMAKE_INSTALL_LIBRARY}
ARCHIVE DESTINATION ${CMAKE_INSTALLL_LIBRARY}
RUNTIME DESTINATION ${CMAKE_INSTALL_BINARY}
INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/clumsy_engine)

install(EXPORT clumsy_engine_targets
FILE clumsy_engine_targets.cmake
DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/clumsy_engine
)

export(
	TARGETS clumsy_engine
	FILE clumsy_engine_targets_in_build.cmake
)

include(GenerateExportHeader)
generate_export_header(clumsy_engine)

set_target_properties( sand_box clumsy_engine
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/bin
)

set_target_properties(clumsy_engine PROPERTIES FOLDER "clumsy_engine")