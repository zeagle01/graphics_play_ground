

cmake_minimum_required(VERSION 3.15)

project(clumsy_engine_test)

set(test_files
simulator_test.cpp
stretch_constraint_test.cpp
spring_stretch_test.cpp
system_equations_solver_test.cpp
signal_test.cpp
simulation_data_test.cpp
data_computes_test.cpp
main.cpp
)


add_executable(clumsy_engine_test)

target_sources(clumsy_engine_test PRIVATE ${test_files})

target_compile_features(clumsy_engine_test PRIVATE cxx_std_20)

set_target_properties(clumsy_engine_test PROPERTIES FOLDER clumsy_engine)

set_target_properties(clumsy_engine_test 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/bin
)





target_link_libraries(clumsy_engine_test PUBLIC clumsy_engine)

##########gtest
#find_package(GTest CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
#target_link_libraries(clumsy_engine_test PRIVATE GTest::gmock)

#find_package(GMock CONFIG REQUIRED ${CMAKE_SOURCE_DIR}/Install/lib/cmake/GMock)
#target_link_libraries(soft_render PRIVATE GMock::gmock)

include(${CMAKE_SOURCE_DIR}/cmake/gtest.cmake)
link_to_gtest(clumsy_engine_test ${CMAKE_SOURCE_DIR}/external/gtest)

