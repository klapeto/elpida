project("Elpida Benchmark Entry Point" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Simple Benchmarking Framework"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_ENTRY_POINT_SOURCES
        Main.cpp
        ArgumentsHelper.cpp
        JsonFormatter.cpp
        DefaultFormatter.cpp
        ResultFormatter.cpp
)

# The benchmarks held to code parity (docs/development/code-parity.md): musl-lite for the C library functions and
# mimalloc as the allocator, the same on every platform.
add_library(elpida-entry-point ${ELPIDA_ENTRY_POINT_SOURCES})
target_link_libraries(elpida-entry-point PUBLIC elpida-core elpida-platform musl-lite musl-platform mimalloc)
target_compile_definitions(elpida-entry-point PRIVATE ELPIDA_PARITY_RUNTIME=1)

# The StdLib benchmarks, which measure the platform's own C library and allocator (glibc, msvcrt, bionic): no
# musl-lite, no mimalloc. See docs/development/code-parity.md#the-stdlib-benchmarks.
add_library(elpida-entry-point-system-libc ${ELPIDA_ENTRY_POINT_SOURCES})
target_link_libraries(elpida-entry-point-system-libc PUBLIC elpida-core elpida-platform)