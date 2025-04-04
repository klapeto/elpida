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
        RegexTask.cpp
        RegexBenchmark.cpp
        Plugin.cpp)

add_executable(elpida-stdlib-benchmarks ${ELPIDA_STDLIB_BENCHMARKS_SOURCES})

target_link_libraries(elpida-stdlib-benchmarks PRIVATE elpida-core elpida-common-tasks elpida-entry-point)

if (ELPIDA_COMPILER_GCC)
    target_compile_options(elpida-stdlib-benchmarks PRIVATE -fno-builtin -fno-tree-builtin-call-dce -fno-dse -fno-tree-dse -fno-dce -fno-tree-dce -fno-plt)
else()
    message(WARNING "Compiler flags for No dead code elimination are not implemented for this compiler")
endif()

install(TARGETS elpida-stdlib-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})
