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

add_library(CompilationBenchmarks SHARED Dummy.cpp)
target_link_libraries(CompilationBenchmarks
        android
        elpida-core
        elpida-compilation-benchmarks
        musl-lite
        log)

add_library(WebBenchmarks SHARED Dummy.cpp)
target_link_libraries(WebBenchmarks
        android
        elpida-core
        elpida-web-benchmarks
        musl-lite
        log)

add_library(MemoryBenchmarks SHARED Dummy.cpp)
target_link_libraries(MemoryBenchmarks
        android
        elpida-core
        elpida-memory-benchmarks
        musl-lite
        log)

add_library(MathBenchmarks SHARED Dummy.cpp)
target_link_libraries(MathBenchmarks
        android
        elpida-core
        elpida-math-benchmarks
        musl-lite
        log)

add_library(ImageBenchmarks SHARED Dummy.cpp)
target_link_libraries(ImageBenchmarks
        android
        elpida-core
        elpida-image-benchmarks
        musl-lite
        log)

add_library(EncryptionBenchmarks SHARED Dummy.cpp)
target_link_libraries(EncryptionBenchmarks
        android
        elpida-core
        elpida-encryption-benchmarks
        musl-lite
        log)

add_library(CompressionBenchmarks SHARED Dummy.cpp)
target_link_libraries(CompressionBenchmarks
        android
        elpida-core
        elpida-compression-benchmarks
        musl-lite
        log)

add_library(StdLibBenchmarks SHARED Dummy.cpp)
target_link_libraries(StdLibBenchmarks
        android
        elpida-core
        elpida-stdlib-benchmarks
        musl-lite
        log)