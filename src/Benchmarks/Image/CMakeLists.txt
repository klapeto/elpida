project("Elpida image benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida image benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_library(elpida-image-benchmarks
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
        RayTracingTask.cpp
        RayTracingBenchmark.cpp
        SceneCreator.cpp)

target_include_directories(elpida-image-benchmarks PRIVATE
        ${ZLIB_INCLUDE_DIRECTORIES}
        ${LIBPNG_INCLUDE_DIRECTORIES}
)

target_link_libraries(elpida-image-benchmarks PUBLIC elpida-core elpida-common-tasks elpida-xml elpida-svg png_static zlibstatic elpida-math elpida-graphics musl-lite)
