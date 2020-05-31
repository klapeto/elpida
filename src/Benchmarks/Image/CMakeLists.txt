project(ImageBenchmarks  VERSION 0.0.1
        DESCRIPTION "Image Benchmarks for Elpida"
        LANGUAGES CXX C)

set(IMAGE_BENCHMARKS_SOURCES
        ConvertToUInt8/ConvertToUInt8.cpp
        ConvertToUInt8/ConvertToUInt8Specification.cpp

        ConvertToFloat/ConvertToFloat.cpp
        ConvertToFloat/ConvertToFloatSpecification.cpp

        FloydSteinberg/FloydSteinbergSpecification.cpp
        FloydSteinberg/FloydSteinberg.cpp

        GrayscaleAverage/GrayscaleAverage.cpp
        GrayscaleAverage/GrayscaleAverageSpecification.cpp

        PngEncoding/PngEncoding.cpp
        PngEncoding/PngEncodingSpecification.cpp

        PngDecoding/PngDecoding.cpp
        PngDecoding/PngDecodingSpecification.cpp

        Encoders/LibPngEncoder.cpp

        Plugin.cpp
        ImageTaskBase.cpp
        ImageDataPropertiesTransformer.cpp
        )


add_library(imageBenchmarks MODULE ${IMAGE_BENCHMARKS_SOURCES})

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(imageBenchmarks PRIVATE -O3)
endif()

target_include_directories(imageBenchmarks PRIVATE "${TASK_BATCHES_CONFIG_INCLUDE_DIR}")
target_link_libraries(imageBenchmarks elpida png)

install(TARGETS imageBenchmarks DESTINATION ${ELPIDA_TASK_BATCH_INSTALL_DIR})