project("Elpida math benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida math benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_executable(elpida-math-benchmarks
        Plugin.cpp
        FFTCalculator.cpp
        CalculateFFTInPlaceTask.cpp
        CalculateFFTInPlaceBenchmark.cpp
        DynamicMatrixMultiplicationTask.cpp
        DynamicMatrixMultiplicationBenchmark.cpp
        MatrixMultiplication32Task.cpp
        MatrixMultiplication32Benchmark.cpp
        MatrixInverse4Task.cpp
        MatrixInverse4Benchmark.cpp
)

target_link_libraries(elpida-math-benchmarks PRIVATE elpida-core elpida-platform elpida-entry-point elpida-math)

install(TARGETS elpida-math-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})