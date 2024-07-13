project(Elpida-Tests)
add_executable(elpida-tests
        Svg/SvgColor.Test.cpp
        Svg/SvgNumber.Test.cpp
        Svg/CharacterStream.Test.cpp
        Svg/XmlParser.Test.cpp
        Svg/SvgCordinate.Test.cpp
        Svg/SvgViewBox.Test.cpp
        Svg/SvgPreserveAspectRatio.Test.cpp
        Svg/SvgTransform.Test.cpp
        Svg/SvgStyle.Test.cpp
        Svg/SvgDocument.Test.cpp
        Svg/SvgBlendMode.Test.cpp
        Svg/SvgBounds.Test.cpp
        Svg/SvgPathGenerator.Test.cpp
)

target_link_libraries(elpida-tests gtest gtest_main elpida-svg elpida-xml elpida-core)