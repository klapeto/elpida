project("Elpida Compression benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Compression benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

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

add_library(elpida-compression-benchmarks SHARED ${ELPIDA_COMPRESSION_BENCHMARKS_SOURCES})

target_link_libraries(elpida-compression-benchmarks PRIVATE elpida-core elpida-common-tasks zlib)

install(TARGETS elpida-compression-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})
