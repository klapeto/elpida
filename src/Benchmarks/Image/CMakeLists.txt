project("Elpida image benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida image benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

# The SVG rasterization benchmark document, embedded so every platform renders the same bytes without an asset file
set(SVG_BENCHMARK_DOCUMENT "${CMAKE_CURRENT_SOURCE_DIR}/Assets/SvgBenchmarkDocument.svg")
set(SVG_BENCHMARK_DOCUMENT_CPP "${CMAKE_CURRENT_BINARY_DIR}/SvgBenchmarkDocument.cpp")
add_custom_command(
        OUTPUT "${SVG_BENCHMARK_DOCUMENT_CPP}"
        COMMAND ${CMAKE_COMMAND}
        -DINPUT=${SVG_BENCHMARK_DOCUMENT}
        -DOUTPUT=${SVG_BENCHMARK_DOCUMENT_CPP}
        -DNAMESPACE=Elpida::Benchmarks::Image
        -DNAME=SvgBenchmarkDocument
        -P "${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake-modules/EmbedFile.cmake"
        DEPENDS "${SVG_BENCHMARK_DOCUMENT}" "${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake-modules/EmbedFile.cmake"
        COMMENT "Embedding SvgBenchmarkDocument.svg"
        VERBATIM)

add_library(elpida-image-benchmarks
        "${SVG_BENCHMARK_DOCUMENT_CPP}"
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

if (ELPIDA_INSTALL_PREDEFINED_BENCHMARK_LIBRARIES)
    install(TARGETS elpida-image-benchmarks ARCHIVE)
endif ()