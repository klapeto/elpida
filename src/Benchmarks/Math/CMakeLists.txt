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
        MatrixMultiplicationTask.hpp
        StaticMatrix.cpp
        Matrix.cpp
        MatrixMultiplicationBenchmark.cpp
)

#target_compile_options(elpida-math-benchmarks PRIVATE -O3)

target_link_libraries(elpida-math-benchmarks PRIVATE elpida-core elpida-platform elpida-entry-point)

install(TARGETS elpida-math-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})