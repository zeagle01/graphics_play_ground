#########glm



function(link_to_glm target_to_be_link glm_repo_dir)
    FetchContent_Declare(
    glm_repo
    GIT_REPOSITORY ${glm_repo_dir}
    GIT_TAG        master
    )
    FetchContent_MakeAvailable(glm_repo)
    target_link_libraries(${target_to_be_link} PRIVATE glm)
endfunction()