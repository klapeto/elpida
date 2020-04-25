cmake_minimum_required(VERSION 3.10)

project(TestBenchmark VERSION 0.0.1
        DESCRIPTION "Test Benchmark for Elpida"
        LANGUAGES CXX C)

set(TESTING_BENCHMARK_SOURCES
        Plugin.cpp
        )

add_library(testTasks MODULE ${TESTING_BENCHMARK_SOURCES})

target_include_directories(testTasks PRIVATE "${TASK_BATCHES_CONFIG_INCLUDE_DIR}")
target_link_libraries(testTasks elpida)

install(TARGETS testTasks DESTINATION ${ELPIDA_TASK_BATCH_INSTALL_DIR})