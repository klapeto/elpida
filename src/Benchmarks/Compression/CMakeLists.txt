project("Elpida Compression benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Compression benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_library(elpida-compression-benchmarks
        Plugin.cpp
        ZlibCompressionTask.cpp
        ZlibCompressionBenchmark.cpp
        ZlibDecompressionTask.cpp
        ZlibDecompressionBenchmark.cpp
)

target_include_directories(elpida-compression-benchmarks PRIVATE
        ${ZLIB_INCLUDE_DIRECTORIES}
)

target_link_libraries(elpida-compression-benchmarks PUBLIC elpida-core elpida-common-tasks zlibstatic musl-lite)
