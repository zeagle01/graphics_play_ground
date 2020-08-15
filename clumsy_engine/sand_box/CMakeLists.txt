
cmake_minimum_required(VERSION 3.15)

project(sand_box)


add_executable(sand_box 
sandbox_app.cpp)

set_target_properties(sand_box PROPERTIES FOLDER clumsy_engine)

target_link_libraries(sand_box PRIVATE clumsy_engine)

target_compile_features(sand_box PRIVATE cxx_std_20)

set_target_properties(sand_box PROPERTIES FOLDER "clumsy_engine")
