
file(GLOB glad_src
    "src/*.*")

add_library(glad ${glad_src})
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(glad PROPERTIES FOLDER external_wrappers)

#target_include_directories(glad 
#    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#    PUBLIC $<INSTALL_INTERFACE:include>
#    )
#
#set(export_target glad)
#set(config_file gladConfig.cmake)
#install(TARGETS glad
#    EXPORT ${export_target}
#    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    )
#install(EXPORT ${export_target}
#    FILE ${config_file}
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${export_target}
#    )
#install(DIRECTORY include/
#    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#    )
#export(TARGETS glad FILE ${config_file})

