
cmake_minimum_required(VERSION 3.15)

project(clumsy_engine)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "export all symbols")

set(source_files 
src/application.cpp
src/event.cpp
src/key_event.cpp
src/key_event.h
src/mouse_event.h
src/mouse_event.cpp
src/application_event.h
src/application_event.cpp
src/profiler.cpp
src/log.cpp
src/window.cpp
src/platform/windows/Windows_window.h
src/platform/windows/Windows_window.cpp
src/helper_utils.h
src/layer.cpp
src/layer_stack.h
src/layer_stack.cpp
src/imgui_layer.cpp
src/handy_helpers.h
src/windows_input.h
src/windows_input.cpp
src/input.cpp
src/dispatcher.h
src/dispatcher.cpp

src/graphic_context.h
src/opengl_context.h
src/opengl_context.cpp


src/simulator.h
src/simulator.cpp
src/interaction.h
src/interaction.cpp
src/gravity.h
src/gravity.cpp
src/inertial.h
src/inertial.cpp
src/system_equations_solver.h
src/system_equations_solver.cpp
src/simulation_data.h
src/simulation_data.cpp
src/spring_stretch.h
src/spring_stretch.cpp
src/one_point_stencils.h
src/one_point_stencils.cpp
src/signal.h
src/type_map.h
src/type_map.cpp
src/type_list.h
src/type_list.cpp
src/class_reflect.h
src/data_computes.h
src/data_computes.cpp
)

set(test_files
test/simulator_test.cpp
test/stretch_constraint_test.cpp
test/spring_stretch_test.cpp
test/system_equations_solver_test.cpp
test/signal_test.cpp
test/simulation_data_test.cpp
test/data_computes_test.cpp
)

set(include_files
include/clumsy_engine/clumsy_engine.h
include/clumsy_engine/application.h
include/clumsy_engine/entry_point.h
include/clumsy_engine/log.h
include/clumsy_engine/profiler.h
include/clumsy_engine/window.h
include/clumsy_engine/event.h
include/clumsy_engine/layer.h
include/clumsy_engine/imgui_layer.h
include/clumsy_engine/key_codes.h
include/clumsy_engine/mouse_button_codes.h
include/clumsy_engine/input.h
)

add_library(clumsy_engine SHARED
${include_files}
${source_files}
${test_files}

)


set_target_properties(clumsy_engine PROPERTIES FOLDER clumsy_engine)




target_compile_features(clumsy_engine PRIVATE cxx_std_20)

target_precompile_headers(clumsy_engine PUBLIC src/pch.h)

include(GenerateExportHeader)
generate_export_header(clumsy_engine)

target_include_directories(clumsy_engine
PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/clumsy_engine>
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/external/spdlog/include>
$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
$<INSTALL_INTERFACE:include>
PRIVATE
#${CMAKE_CURRENT_BINARY_DIR}
src
)




#hunter_add_package(GLFW)
#find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(clumsy_engine PRIVATE glfw)


#
target_link_libraries(clumsy_engine PRIVATE glad)


include(${CMAKE_SOURCE_DIR}/cmake/glm.cmake)
link_to_glm(clumsy_engine ${CMAKE_SOURCE_DIR}/external/glm)

##########imgui
target_link_libraries(clumsy_engine PUBLIC my_imgui)

##########gtest
include(${CMAKE_SOURCE_DIR}/cmake/gtest.cmake)
link_to_gtest(clumsy_engine ${CMAKE_SOURCE_DIR}/external/gtest)


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
