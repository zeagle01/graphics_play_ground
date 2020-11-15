
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was glfw3Config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

find_package(Threads REQUIRED)

if("1")
  find_package(OpenGL REQUIRED)
endif()

if("")
  find_package(OpenGL REQUIRED)
  find_package(xcursor CONFIG REQUIRED)
  find_package(xinerama CONFIG REQUIRED)
  find_package(xrandr CONFIG REQUIRED)
  find_package(xi CONFIG REQUIRED)
endif()

if("")
  find_package(appkit REQUIRED)
  find_package(coredata REQUIRED)
  find_package(foundation REQUIRED)
  find_package(iokit REQUIRED)
  find_package(corefoundation REQUIRED)
  find_package(corevideo REQUIRED)
endif()

if("")
  if(NOT "OFF")
    find_package(OpenGL REQUIRED)
  endif()
endif()

include("${CMAKE_CURRENT_LIST_DIR}/glfw3Targets.cmake")
check_required_components("glfw3")
