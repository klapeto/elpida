cmake_minimum_required(VERSION 3.22.1)

project("Elpida")

set(ELPIDA_ENABLE_BENCHMARK_TOOLS ON)

add_link_options(
        "-Wl,-z,max-page-size=16384"
        "-Wl,-z,common-page-size=16384"
)
link_directories(
        ${CMAKE_SYSROOT}/usr/lib/${CMAKE_SYSTEM_PROCESSOR}-linux-android
)

add_subdirectory(../../../../../.. ${CMAKE_CURRENT_BINARY_DIR}/elpida)

include_directories(Elpida PRIVATE ../../../../../../include ../../../../../../apps/Application)

add_library(Elpida SHARED Elpida.cpp DynamicLoadedBenchmark.cpp InProcessBenchmarkExecutionService.cpp)
target_link_libraries(Elpida
        android
        elpida-core
        elpida-application
        elpida-overheads-calculator
        elpida-platform
        elpida-shared
        log)

add_library(compilation-benchmarks-lib MODULE Dummy.cpp)
target_link_libraries(compilation-benchmarks-lib
        elpida-core
        elpida-compilation-benchmarks
        musl-lite)

add_library(web-benchmarks-lib MODULE Dummy.cpp)
target_link_libraries(web-benchmarks-lib
        elpida-core
        elpida-web-benchmarks
        musl-lite)

add_library(memory-benchmarks-lib MODULE Dummy.cpp)
target_link_libraries(memory-benchmarks-lib
        elpida-core
        elpida-memory-benchmarks
        musl-lite)

add_library(math-benchmarks-lib MODULE Dummy.cpp)
target_link_libraries(math-benchmarks-lib
        elpida-core
        elpida-math-benchmarks
        musl-lite)

add_library(image-benchmarks-lib MODULE Dummy.cpp)
target_link_libraries(image-benchmarks-lib
        elpida-core
        elpida-image-benchmarks
        musl-lite)

add_library(encryption-benchmarks-lib MODULE Dummy.cpp)
target_link_libraries(encryption-benchmarks-lib
        elpida-core
        elpida-encryption-benchmarks
        musl-lite)

add_library(compression-benchmarks-lib MODULE Dummy.cpp)
target_link_libraries(compression-benchmarks-lib
        elpida-core
        elpida-compression-benchmarks
        musl-lite)

add_library(stdLib-benchmarks-lib MODULE Dummy.cpp)
target_link_libraries(stdLib-benchmarks-lib
        elpida-core
        elpida-stdlib-benchmarks
        musl-lite)
