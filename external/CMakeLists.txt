target_include_directories(Xu PUBLIC nano-signal-slot)
target_include_directories(Xu PRIVATE earcut/include)

if (${XU_QUICK_MODULE})
    # Build GLFW
    set(GLFW_BUILD_DOCS OFF)
    set(GLFW_BUILD_EXAMPLES OFF)
    set(GLFW_BUILD_TESTS OFF)

    add_subdirectory(glfw)
    
    # Generate and Build GLAD
    set(GLAD_PROFILE "core")
    set(GLAD_API "gl=4.3")
    add_subdirectory(glad)
    
    target_link_libraries(Xu PUBLIC glad glfw)
endif()
