
cmake_minimum_required(VERSION 3.26)

project(sim_lib)

add_library(sim_lib)

set_target_properties(sim_lib PROPERTIES FOLDER common)


target_sources(sim_lib PUBLIC
    FILE_SET CXX_MODULES FILES
	simulator.cpp
	simulator.ixx 
	sim_data.ixx 
	simulator_imp.cpp
	solver.cpp
	simulator_datas.cpp
	solver_data_update.cpp
	solver_dummy.cpp
	simulator_data_update.cpp
	struct_functors.cpp
	small_mat.cpp
	constraint_kernels.cpp
)

### moduel support end

target_compile_features(sim_lib PRIVATE cxx_std_20)

########## ce_log
target_link_libraries(sim_lib PRIVATE ce_log)

########## clumsy_lib
target_link_libraries(sim_lib PRIVATE clumsy_lib)

########## matrix_math
target_link_libraries(sim_lib PRIVATE matrix_math)

########## parallel_lib
target_link_libraries(sim_lib PRIVATE parallel_lib)

