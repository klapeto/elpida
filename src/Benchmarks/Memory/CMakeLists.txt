project(MemoryBenchmarks VERSION 0.0.1
        DESCRIPTION "Memory Benchmarks for Elpida"
        LANGUAGES CXX C)

set(MEMORY_BENCHMARKS_SOURCES
        Plugin.cpp
        WorkingSetSizes.cpp
        #Latency/MemoryReadLatency.cpp
        #Latency/MemoryReadLatencySpecification.cpp
        Read/MemoryRead.cpp
        Read/MemoryReadSpecification.cpp)


add_library(memoryBenchmarks MODULE ${MEMORY_BENCHMARKS_SOURCES})

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(memoryBenchmarks PRIVATE -O3)
endif()

target_include_directories(memoryBenchmarks PRIVATE "${TASK_BATCHES_CONFIG_INCLUDE_DIR}")
target_link_libraries(memoryBenchmarks elpida)

install(TARGETS memoryBenchmarks DESTINATION ${ELPIDA_TASK_BATCH_INSTALL_DIR})
