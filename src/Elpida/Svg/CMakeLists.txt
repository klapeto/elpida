project("Elpida Svg")

add_library(elpida-svg
        SvgBounds.cpp
        SvgColor.cpp
        SvgLength.cpp
        SvgDocument.cpp
        SvgGradientStop.cpp
        SvgNumber.cpp
        SvgPath.cpp
        SvgPreserveAspectRatio.cpp
        SvgTransform.cpp
        SvgViewBox.cpp
        SvgStyle.cpp
        SvgElement.cpp
        SvgFill.cpp
        SvgStroke.cpp
        SvgVisibility.cpp
        SvgShape.cpp
        SvgElementFactory.cpp
        SvgRadialGradient.cpp
        SvgGradient.cpp
        SvgLinearGradient.cpp
        SvgDefs.cpp
        SvgSvgElement.cpp
        SvgPathCommand.cpp
        SvgRect.cpp
        SvgFontSize.cpp
        ../../../include/Elpida/Svg/SvgFontSize.hpp
        SvgRasterizer.cpp
        ../../../include/Elpida/Svg/SvgRasterizer.hpp
)

target_link_libraries(elpida-svg PRIVATE elpida-xml)