
cmake_minimum_required(VERSION 3.15)


project(sand_box)


add_executable(sand_box 
sandbox_app.cpp)

target_link_libraries(sand_box PRIVATE clumsy_engine)

set_target_properties(sand_box PROPERTIES FOLDER "clumsy_engine")
