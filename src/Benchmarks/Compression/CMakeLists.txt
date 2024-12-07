project("Elpida Compression benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Compression benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_COMPRESSION_BENCHMARKS_SOURCES
        Plugin.cpp
        ZlibCompressionTask.cpp
        ZlibCompressionBenchmark.cpp
        ZlibCompressionBenchmark.hpp
        ZlibDecompressionTask.cpp
        ZlibDecompressionTask.hpp
        ZlibDecompressionBenchmark.cpp
        ZlibDecompressionBenchmark.hpp
        ${MUSL_LITE_SOURCES}
)

add_executable(elpida-compression-benchmarks ${ELPIDA_COMPRESSION_BENCHMARKS_SOURCES})

target_link_libraries(elpida-compression-benchmarks PRIVATE elpida-core elpida-common-tasks zlibstatic elpida-entry-point)

install(TARGETS elpida-compression-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})
