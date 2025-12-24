project("Elpida memory benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida memory benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)


add_library(elpida-memory-benchmarks
        MemoryLatencyBenchmark.cpp
        MemoryLatencyTask.cpp
        MemoryReadBandwidthBenchmark.cpp
        MemoryReadBandwidthTask.cpp
        Plugin.cpp
)

target_link_libraries(elpida-memory-benchmarks PUBLIC elpida-core elpida-common-tasks)

if (ELPIDA_INSTALL_PREDEFINED_BENCHMARK_LIBRARIES)
    install(TARGETS elpida-memory-benchmarks ARCHIVE)
endif ()