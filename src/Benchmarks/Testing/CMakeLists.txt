cmake_minimum_required(VERSION 3.10)

project("Testing Benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Test Benchmark for Elpida"
        LANGUAGES CXX C)

set(TESTING_BENCHMARK_SOURCES
        Plugin.cpp
        )

if (MSYS OR MINGW)
    generate_product_version(
            TESTING_BENCHMARK_SOURCES
            NAME ${PROJECT_NAME}
            ICON ${ELPIDA_ROOT_DIRECTORY}/images/Elpida-icon.ico
            VERSION_MAJOR ${ELPIDA_VERSION_MAJOR}
            VERSION_MINOR ${ELPIDA_VERSION_MINOR}
            VERSION_PATCH ${ELPIDA_VERSION_REVISION}
            VERSION_REVISION ${ELPIDA_VERSION_BUILD}
            COMPANY_NAME ${ELPIDA_COMPANY_NAME}
            FILE_DESCRIPTION ${PROJECT_DESCRIPTION}
    )
endif()

add_library(testBenchmarks MODULE ${TESTING_BENCHMARK_SOURCES})

target_include_directories(testBenchmarks PRIVATE "${TASK_BATCHES_CONFIG_INCLUDE_DIR}")
target_link_libraries(testBenchmarks elpida)


install(TARGETS testBenchmarks DESTINATION ${ELPIDA_TASK_BATCH_INSTALL_DIR})