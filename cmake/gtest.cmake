


function(link_to_gtest target_to_be_linked gtest_repo)

	include(FetchContent)

	FetchContent_Declare(
	  googletest
	  GIT_REPOSITORY ${gtest_repo}
	  GIT_TAG        master
	)

	# this one is important for windows
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  

	FetchContent_MakeAvailable(googletest)

	set_target_properties(gmock_main PROPERTIES EXCLUDE_FROM_ALL TRUE)
	target_link_libraries(${target_to_be_linked} PRIVATE gmock_main)

endfunction()
