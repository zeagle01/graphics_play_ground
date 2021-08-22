
cmake_minimum_required(VERSION 3.15)

project(clumsy_engine)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "export all symbols")

set(source_files 
src/application.cpp
src/event.cpp
src/key_event.cpp
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
src/dispatcher.cpp
src/time_step.cpp
src/file_dialogs.cpp

src/graphic_context.h
src/opengl_context.h
src/opengl_context.cpp
src/shader.cpp
src/buffer.cpp
src/opengl_buffer.cpp
src/renderer.cpp
src/enum_loop.h
src/vertex_array.cpp
src/openGL_vertex_array.h
src/openGL_vertex_array.cpp
src/renderer_API.cpp
src/render_command.cpp
src/openGL_Renderer_API.h
src/openGL_Renderer_API.cpp
src/openGL_shader.cpp
src/texture.cpp
src/openGL_texture.h
src/openGL_texture.cpp
src/camara.cpp


src/simulator.cpp
src/interaction.cpp
src/gravity.cpp
src/inertial.cpp
src/system_equations_solver.h
src/system_equations_solver.cpp
src/linear_equations_solver.h
src/linear_solver_Jacobi.h
src/linear_solver_Jacobi.cpp
src/linear_solver_CG.h
src/linear_solver_CG.cpp
src/simulation_data.cpp
src/spring_stretch.h
src/spring_stretch.cpp
src/discrete_collision.h
src/discrete_collision.cpp
src/one_point_stencils.h
src/one_point_stencils.cpp
src/signal.h
src/data_computes.h
src/data_computes.cpp
src/mesh_loader.cpp
src/compute_hessian_gradient.h
src/compute_hessian_gradient.cpp
src/discrete_collision_detection.h
src/discrete_collision_detection.cpp
src/AABB.h

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
include/clumsy_engine/renderer.h
include/clumsy_engine/render_command.h
include/clumsy_engine/renderer_API.h
include/clumsy_engine/buffer.h
include/clumsy_engine/shader.h
include/clumsy_engine/openGL_shader.h
include/clumsy_engine/vertex_array.h
include/clumsy_engine/texture.h
include/clumsy_engine/camara.h
include/clumsy_engine/dispatcher.h
include/clumsy_engine/key_event.h
include/clumsy_engine/mouse_event.h
include/clumsy_engine/time_step.h
include/clumsy_engine/ref.h
include/clumsy_engine/file_dialogs.h

include/simulator/simulator.h
include/simulator/simulation_data.h
include/simulator/inertial.h
include/simulator/interaction.h
include/simulator/simulation_interactions.h
include/simulator/gravity.h
include/simulator/mesh_loader.h
)

add_library(clumsy_engine  SHARED
${include_files}
${source_files}
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
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/simulator>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
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


## glm
find_package(glm CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
target_link_libraries(clumsy_engine PUBLIC glm::glm)

########## imgui
target_link_libraries(clumsy_engine PUBLIC my_imgui)


## matrix_math
target_link_libraries(clumsy_engine PUBLIC matrix_math)

## clumsy_lib
target_link_libraries(clumsy_engine PUBLIC clumsy_lib)

########## stb
target_link_libraries(clumsy_engine PUBLIC my_stb)




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
