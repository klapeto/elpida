set(GENERAL_TASKS_SOURCES
        AlignedMemory.cpp
        AllocateMemory.cpp
        Memory.cpp
        MemoryChunk.cpp
        NumaAllocatePerThread.cpp
        NumaMemory.cpp
        Plugin.cpp
        ReadFile.cpp
        UnalignedMemory.cpp
        WriteFile.cpp)

add_library(generalTasks ${GENERAL_TASKS_SOURCES})
target_include_directories(generalTasks PUBLIC "${ELPIDA_INCLUDE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(generalTasks elpida)