project("Elpida Compilation benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Compilation benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_COMPILATION_BENCHMARKS_SOURCES
        Plugin.cpp
        GenerateCppCodeTask.cpp
        CompileWithClangTask.cpp
        ClangCompilationBenchmark.cpp
)

add_executable(elpida-compilation-benchmarks ${ELPIDA_COMPILATION_BENCHMARKS_SOURCES})

#set(Clang_DIR /usr/lib/cmake/clang-18)

find_package(Clang REQUIRED CONFIG)

#target_include_directories(elpida-compilation-benchmarks PRIVATE /usr/lib/llvm-18/include)

target_link_libraries(elpida-compilation-benchmarks PRIVATE elpida-core elpida-common-tasks elpida-entry-point musl-lite libclang clangCodeGen LLVMX86CodeGen)

install(TARGETS elpida-compilation-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})