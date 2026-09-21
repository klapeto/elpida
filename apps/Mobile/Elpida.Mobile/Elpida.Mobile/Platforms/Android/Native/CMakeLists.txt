cmake_minimum_required(VERSION 3.22.1)

project("Elpida")

add_subdirectory(../../../../../../.. ${CMAKE_CURRENT_BINARY_DIR}/elpida)

add_link_options(
        "-Wl,-z,max-page-size=16384"
        "-Wl,-z,common-page-size=16384"
)
include_directories(Elpida PRIVATE ../../../../../../../include ../../../../../../../apps/Application)

link_directories(
        ${CMAKE_SYSROOT}/usr/lib/${CMAKE_SYSTEM_PROCESSOR}-linux-android
)

add_library(Elpida SHARED Elpida.cpp InProcessBenchmarkExecutionService.cpp DynamicLoadedBenchmark.cpp)
target_link_libraries(Elpida
        android
        elpida-core
        elpida-application
        elpida-overheads-calculator
        elpida-platform
        elpida-shared
        musl-lite
        dl
        log)

add_library(CompilationBenchmarks MODULE Dummy.cpp)
target_link_libraries(CompilationBenchmarks
        elpida-core
        elpida-compilation-benchmarks
        musl-lite)

add_library(WebBenchmarks MODULE Dummy.cpp)
target_link_libraries(WebBenchmarks
        elpida-core
        elpida-web-benchmarks
        musl-lite)

add_library(MemoryBenchmarks MODULE Dummy.cpp)
target_link_libraries(MemoryBenchmarks
        elpida-core
        elpida-memory-benchmarks
        musl-lite)

add_library(MathBenchmarks MODULE Dummy.cpp)
target_link_libraries(MathBenchmarks
        elpida-core
        elpida-math-benchmarks
        musl-lite)

add_library(ImageBenchmarks MODULE Dummy.cpp)
target_link_libraries(ImageBenchmarks
        elpida-core
        elpida-image-benchmarks
        musl-lite)

add_library(EncryptionBenchmarks MODULE Dummy.cpp)
target_link_libraries(EncryptionBenchmarks
        elpida-core
        elpida-encryption-benchmarks
        musl-lite)

add_library(CompressionBenchmarks MODULE Dummy.cpp)
target_link_libraries(CompressionBenchmarks
        elpida-core
        elpida-compression-benchmarks
        musl-lite)

add_library(StdLibBenchmarks MODULE Dummy.cpp)
target_link_libraries(StdLibBenchmarks
        elpida-core
        elpida-stdlib-benchmarks
        musl-lite)