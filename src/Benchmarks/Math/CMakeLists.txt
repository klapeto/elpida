project("Elpida math benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida math benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_executable(elpida-math-benchmarks
        Plugin.cpp
        FFTCalculator.cpp
        CalculateFFTInPlaceTask.cpp
        CalculateFFTInPlaceBenchmark.cpp
        MatrixMultiplicationTask.cpp
        StaticMatrix.cpp
        Matrix.cpp
        MatrixMultiplicationBenchmark.cpp
)

target_link_libraries(elpida-math-benchmarks PRIVATE musl elpida-core elpida-platform elpida-entry-point)

install(TARGETS elpida-math-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})