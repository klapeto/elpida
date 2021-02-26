add_subdirectory(Image)
add_subdirectory(Memory)
add_subdirectory(Testing)

if (ELPIDA_DEBUG_BUILD EQUAL 1)
    set(TASK_BATCH_DEBUG_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Debug batch path" FORCE)
endif()