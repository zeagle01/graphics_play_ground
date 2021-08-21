
cmake_minimum_required(VERSION 3.15)

project(sand_box)


add_executable(sand_box 
sandbox_app.cpp
sim_app.h
sim_app.cpp
shader_sources.h
ui_map_to_simulation_records.h
ui_simulation_mapper_helpers.h
)

set_target_properties(sand_box PROPERTIES FOLDER clumsy_engine)

target_link_libraries(sand_box PUBLIC clumsy_engine)

target_compile_features(sand_box PRIVATE cxx_std_20)

set_target_properties(sand_box PROPERTIES FOLDER "clumsy_engine")
