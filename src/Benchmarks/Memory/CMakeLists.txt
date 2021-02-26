project("Memory Benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Memory Benchmarks for Elpida"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(MEMORY_BENCHMARKS_SOURCES
        Plugin.cpp
        WorkingSetSizes.cpp
        Latency/MemoryLatency.cpp
        Latency/MemoryLatencySpecification.cpp
        Bandwidth/MemoryReadBandwidth.cpp
        Bandwidth/MemoryReadBandwidthSpecification.cpp)

if (MSYS OR MINGW)
    generate_product_version(
            MEMORY_BENCHMARKS_SOURCES
            NAME ${PROJECT_NAME}
            ICON ${ELPIDA_ROOT_DIRECTORY}/images/Elpida-icon.ico
            VERSION_MAJOR ${ELPIDA_VERSION_MAJOR}
            VERSION_MINOR ${ELPIDA_VERSION_MINOR}
            VERSION_PATCH ${ELPIDA_VERSION_REVISION}
            VERSION_REVISION ${ELPIDA_VERSION_BUILD}
            COMPANY_NAME ${ELPIDA_COMPANY_NAME}
            FILE_DESCRIPTION ${PROJECT_DESCRIPTION}
    )
endif()

add_library(memoryBenchmarks MODULE ${MEMORY_BENCHMARKS_SOURCES})

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(memoryBenchmarks PRIVATE -O3)
endif()

#target_compile_options(memoryBenchmarks PRIVATE -O3)

target_include_directories(memoryBenchmarks PRIVATE "${TASK_BATCHES_CONFIG_INCLUDE_DIR}")
target_link_libraries(memoryBenchmarks elpida)

install(TARGETS memoryBenchmarks DESTINATION ${ELPIDA_TASK_BATCH_INSTALL_DIR})
