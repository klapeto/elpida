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

if (ELPIDA_COMPILER_GCC)
    target_compile_options(elpida-memory-benchmarks PRIVATE -fno-dce -fno-tree-dce)
else()
    message(WARNING "Compiler flags for No dead code elimination are not implemented for this compiler")
endif()

install(TARGETS elpida-memory-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})
