project("Elpida image benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida image benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_IMAGE_BENCHMARKS_SOURCES
        PngEncodingTask.cpp
        PngDecodingTask.cpp
        PngEncodingDecodingBenchmark.cpp
        ConvertToFloatTask.cpp
        ConvertToGrayscaleTask.cpp
        ConvertToUInt8Task.cpp
        ImageTaskData.cpp
        SvgParseTask.cpp
        SvgRasterization.cpp
        SvgBenchmark.cpp
        nanosvg.c
        nanosvgrast.c
        Plugin.cpp
        Svg/SvgCoordinate.cpp
        Svg/SvgDocument.cpp
        Svg/SvgGradient.cpp
        Svg/SvgGradientData.cpp
        Svg/SvgGradientStop.cpp
        Svg/SvgLinearData.cpp
        Svg/SvgPaint.cpp
        Svg/SvgPath.cpp
        Svg/SvgRadialData.cpp
        Svg/SvgShape.cpp
        Svg/SvgAttribute.cpp
        Svg/SvgAttributePair.cpp

        )

add_library(elpida-image-benchmarks SHARED ${ELPIDA_IMAGE_BENCHMARKS_SOURCES})

set_target_properties(elpida-image-benchmarks PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(elpida-image-benchmarks PRIVATE elpida-core elpida-common-tasks png)

#target_compile_options(elpida-image-benchmarks PRIVATE -O3)
install(TARGETS elpida-image-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})
