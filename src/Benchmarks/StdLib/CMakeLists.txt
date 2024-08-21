project("Elpida StdLib benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida StdLib benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_STDLIB_BENCHMARKS_SOURCES
        StrtodTask.cpp
        StrtodBenchmark.cpp
        MemsetTask.cpp
        MemsetBenchmark.cpp
        MemcpyTask.cpp
        MemcpyBenchmark.cpp
        MemchrTask.cpp
        MemchrBenchmark.cpp
        StrtoulTask.cpp
        StrtoulBenchmark.cpp
        SqrtTask.cpp
        SqrtBenchmark.cpp
        CeilTask.cpp
        CeilBenchmark.cpp
        FloorTask.cpp
        FloorBenchmark.cpp
        Plugin.cpp)

add_executable(elpida-stdlib-benchmarks ${ELPIDA_STDLIB_BENCHMARKS_SOURCES})

target_compile_options(elpida-stdlib-benchmarks PRIVATE -fno-builtin -fno-tree-builtin-call-dce -fno-dse -fno-tree-dse -fno-dce -fno-tree-dce -fno-plt)
target_link_libraries(elpida-stdlib-benchmarks PRIVATE musl elpida-core elpida-common-tasks elpida-entry-point)

install(TARGETS elpida-stdlib-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})
