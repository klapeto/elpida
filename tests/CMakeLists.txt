project(Elpida-Tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(elpida-tests
        Svg/SvgColor.Test.cpp
        Svg/SvgNumber.Test.cpp
        Svg/CharacterStream.Test.cpp
        Svg/XmlParser.Test.cpp
        Svg/SvgCordinate.Test.cpp)

target_include_directories(elpida-tests PUBLIC ${ELPIDA_ROOT_DIRECTORY}/src/Benchmarks/Image)
target_link_libraries(elpida-tests gtest gtest_main elpida-image-benchmarks)