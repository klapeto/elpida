set(ELPIDA_COMPRESSION_BENCHMARKS_SOURCES
        Plugin.cpp
        ZlibCompressionTask.cpp
        ZlibCompressor.cpp
        ZlibCompressor.hpp
        ZlibCompressionBenchmark.cpp
        ZlibCompressionBenchmark.hpp
        ZlibDecompressionTask.cpp
        ZlibDecompressionTask.hpp
        ZlibDecompressionBenchmark.cpp
        ZlibDecompressionBenchmark.hpp
)

add_executable(elpida-compression-benchmarks ${ELPIDA_COMPRESSION_BENCHMARKS_SOURCES})

target_link_libraries(elpida-compression-benchmarks PRIVATE elpida-core elpida-common-tasks zlibstatic elpida-entry-point)

install(TARGETS elpida-compression-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})
