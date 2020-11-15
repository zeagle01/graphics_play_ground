#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PNG::png" for configuration "Debug"
set_property(TARGET PNG::png APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PNG::png PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/pngd.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS PNG::png )
list(APPEND _IMPORT_CHECK_FILES_FOR_PNG::png "${_IMPORT_PREFIX}/lib/pngd.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
