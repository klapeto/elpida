project(MemoryTasks VERSION 0.0.1
        DESCRIPTION "Memory Benchmarks for Elpida"
        LANGUAGES CXX C)

set(MEMORY_TASKS_SOURCES
        Plugin.cpp
        WorkingSetSizes.cpp
#        Latency/MemoryLatency.cpp
#        Latency/MemoryLatencyTaskBatch.cpp
        Read/MemoryRead.cpp
        Read/MemoryReadSpecification.cpp)




add_library(memoryTasks MODULE ${MEMORY_TASKS_SOURCES})

target_include_directories(memoryTasks PRIVATE "${TASK_BATCHES_CONFIG_INCLUDE_DIR}")
target_link_libraries(memoryTasks elpida)

install(TARGETS memoryTasks DESTINATION ${ELPIDA_TASK_BATCH_INSTALL_DIR})
