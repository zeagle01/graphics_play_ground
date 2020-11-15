#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GMock::gmock" for configuration "Release"
set_property(TARGET GMock::gmock APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GMock::gmock PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/gmock.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS GMock::gmock )
list(APPEND _IMPORT_CHECK_FILES_FOR_GMock::gmock "${_IMPORT_PREFIX}/lib/gmock.lib" )

# Import target "GMock::main" for configuration "Release"
set_property(TARGET GMock::main APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GMock::main PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/gmock_main.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS GMock::main )
list(APPEND _IMPORT_CHECK_FILES_FOR_GMock::main "${_IMPORT_PREFIX}/lib/gmock_main.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
