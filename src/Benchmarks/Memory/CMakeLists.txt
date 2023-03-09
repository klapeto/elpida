project("Elpida memory benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida memory benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_MEMORY_BENCHMARKS_SOURCES
        MemoryLatencyBenchmark.cpp
        MemoryLatencyTask.cpp
        MemoryReadBandwidthBenchmark.cpp
        MemoryReadBandwidthTask.cpp
        Plugin.cpp)

add_library(elpida-memory-benchmarks SHARED ${ELPIDA_MEMORY_BENCHMARKS_SOURCES})

target_link_libraries(elpida-memory-benchmarks PUBLIC elpida)

install(TARGETS elpida-memory-benchmarks DESTINATION ${ELPIDA_TASK_BATCH_INSTALL_DIR})
