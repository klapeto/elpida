project("Elpida StdLib benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida StdLib benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_library(elpida-stdlib-benchmarks
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
        Plugin.cpp
)

target_link_libraries(elpida-stdlib-benchmarks PUBLIC elpida-core elpida-common-tasks)
target_compile_options(elpida-stdlib-benchmarks PRIVATE -fno-builtin -fno-plt)

if (ELPIDA_INSTALL_PREDEFINED_BENCHMARK_LIBRARIES)
    install(TARGETS elpida-stdlib-benchmarks ARCHIVE)
endif ()