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

add_executable(elpida-memory-benchmarks ${ELPIDA_MEMORY_BENCHMARKS_SOURCES})

target_link_libraries(elpida-memory-benchmarks PRIVATE elpida-core elpida-common-tasks elpida-entry-point)
target_compile_options(elpida-memory-benchmarks PRIVATE -fno-dce -fno-tree-dce)

install(TARGETS elpida-memory-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})
