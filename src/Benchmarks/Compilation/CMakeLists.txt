project("Elpida Compilation benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Compilation benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_library(elpida-compilation-benchmarks
        Plugin.cpp
        GenerateCppCodeTask.cpp
        CompileWithClangTask.cpp
        ClangCompilationBenchmark.cpp
)

target_include_directories(elpida-compilation-benchmarks PRIVATE
        ${LLVM_INCLUDE_DIRECTORIES}
)
target_link_libraries(elpida-compilation-benchmarks PUBLIC elpida-core elpida-common-tasks clangBasic clangCodeGen LLVMX86CodeGen musl-lite)