include(CMakeFindDependencyMacro)

find_dependency(ZLIB CONFIG)

include("${CMAKE_CURRENT_LIST_DIR}/PNGTargets.cmake")

# ALIAS for imported target requires CMake >= 3.11:
# - https://cmake.org/cmake/help/latest/release/3.11.html#other
if(NOT CMAKE_VERSION VERSION_LESS 3.11 AND NOT TARGET PNG::PNG)
  set_target_properties(
      PNG::png
      PROPERTIES
      IMPORTED_GLOBAL True
  )
  add_library(PNG::PNG ALIAS PNG::png)
endif()
