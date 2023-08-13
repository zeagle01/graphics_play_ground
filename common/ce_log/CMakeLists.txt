
cmake_minimum_required(VERSION 3.26)

project(ce_log)

add_library(ce_log  
	ce_log.h
	ce_log.cpp
)


target_include_directories(ce_log
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/external/spdlog/include>
)

set_target_properties(ce_log PROPERTIES FOLDER common)

target_compile_features(ce_log PRIVATE cxx_std_20)


