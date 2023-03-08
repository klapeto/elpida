project("Elpida image benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida image benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_IMAGE_BENCHMARKS_SOURCES
        PngEncodingTask.cpp
        PngDecodingTask.cpp
        PngEncodingDecodingBenchmark.cpp
        ConvertToFloatTask.cpp ConvertToGrayscaleTask.cpp ConvertToGrayscaleTask.hpp ConvertToUInt8Task.cpp ConvertToUInt8Task.hpp ImageTaskData.cpp ImageTaskData.hpp)

add_library(elpida-image-benchmarks SHARED ${ELPIDA_IMAGE_BENCHMARKS_SOURCES})

set_target_properties(elpida-image-benchmarks PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(elpida-image-benchmarks PRIVATE elpida elpida-common-tasks png)
