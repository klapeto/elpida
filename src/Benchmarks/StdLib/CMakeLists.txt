project("Elpida StdLib benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida StdLib benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_STDLIB_BENCHMARKS_SOURCES
        MallocTask.cpp
        MallocBenchmark.cpp
        StrtodTask.cpp
        StrtodBenchmark.cpp
        MemsetTask.cpp
        MemsetBenchmark.cpp
        Plugin.cpp)

add_executable(elpida-stdlib-benchmarks ${ELPIDA_STDLIB_BENCHMARKS_SOURCES})

target_compile_options(elpida-stdlib-benchmarks PRIVATE -fno-dce -fno-tree-dce)
target_link_libraries(elpida-stdlib-benchmarks PRIVATE elpida-core elpida-common-tasks elpida-entry-point)

install(TARGETS elpida-stdlib-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})
