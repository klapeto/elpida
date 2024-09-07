project("Elpida image benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida image benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_IMAGE_BENCHMARKS_SOURCES
        PngEncodingTask.cpp
        PngDecodingTask.cpp
        PngEncodingBenchmark.cpp
        ConvertToFloatTask.cpp
        ConvertToGrayscaleTask.cpp
        ConvertToUInt8Task.cpp
        ImageTaskData.cpp
        Plugin.cpp
        SvgParseTask.cpp
        SvgParseBenchmark.cpp
        SvgCalculateTask.cpp
        SvgRasterizationTask.cpp
        SvgRasterizationBenchmark.cpp
        SvgDocumentGenerationTask.cpp
        SvgRasterizationFromFileBenchmark.cpp
        SvgRasterization2Task.cpp
        SvgRasterization2Benchmark.cpp
)

add_executable(elpida-image-benchmarks ${ELPIDA_IMAGE_BENCHMARKS_SOURCES})

target_link_libraries(elpida-image-benchmarks PRIVATE elpida-core elpida-common-tasks elpida-xml elpida-svg png elpida-entry-point)

install(TARGETS elpida-image-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})
