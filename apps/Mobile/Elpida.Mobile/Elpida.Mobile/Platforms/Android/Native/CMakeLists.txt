cmake_minimum_required(VERSION 3.22.1)

project("elpida")

add_subdirectory(../../../../../../.. ${CMAKE_CURRENT_BINARY_DIR}/elpida)

add_library(${CMAKE_PROJECT_NAME} SHARED Elpida.cpp InProcessBenchmarkExecutionService.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ../../../../../../../include ../../../../../../../apps/Application)

link_directories(
        ${CMAKE_SYSROOT}/usr/lib/${CMAKE_SYSTEM_PROCESSOR}-linux-android
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        elpida-core
        elpida-application
        elpida-overheads-calculator
        elpida-platform
        elpida-shared
        elpida-compression-benchmarks
        elpida-compilation-benchmarks
        elpida-encryption-benchmarks
        elpida-image-benchmarks
        elpida-math-benchmarks
        elpida-memory-benchmarks
        elpida-stdlib-benchmarks
        elpida-web-benchmarks
        log)