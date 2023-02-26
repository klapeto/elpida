project("Image Benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Image Benchmarks for Elpida"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
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

        Plugin.cpp
        Image.cpp
        Pixel.cpp
        ImageTaskBase.cpp
        ImageDataPropertiesTransformer.cpp
        )


if (MSYS OR MINGW)
    generate_product_version(
            IMAGE_BENCHMARKS_SOURCES
            NAME ${PROJECT_NAME}
            ICON ${ELPIDA_ROOT_DIRECTORY}/images/Elpida-icon.ico
            VERSION_MAJOR ${ELPIDA_VERSION_MAJOR}
            VERSION_MINOR ${ELPIDA_VERSION_MINOR}
            VERSION_PATCH ${ELPIDA_VERSION_REVISION}
            VERSION_REVISION ${ELPIDA_VERSION_BUILD}
            COMPANY_NAME ${ELPIDA_COMPANY_NAME}
            FILE_DESCRIPTION ${PROJECT_DESCRIPTION}
    )
endif()

add_library(imageBenchmarks MODULE ${IMAGE_BENCHMARKS_SOURCES})

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(imageBenchmarks PRIVATE -O3)
endif()


target_include_directories(imageBenchmarks PRIVATE "${TASK_BATCHES_CONFIG_INCLUDE_DIR}")
target_link_libraries(imageBenchmarks elpida-engine elpida-commonTasks png)

install(TARGETS imageBenchmarks DESTINATION ${ELPIDA_TASK_BATCH_INSTALL_DIR})