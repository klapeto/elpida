project("Elpida Svg")

add_library(elpida-svg
        SvgBounds.cpp
        SvgColor.cpp
        SvgLength.cpp
        SvgDocument.cpp
        SvgGradientStop.cpp
        SvgNumber.cpp
        SvgPreserveAspectRatio.cpp
        SvgTransform.cpp
        SvgViewBox.cpp
        SvgStyle.cpp
        SvgElement.cpp
        SvgFill.cpp
        SvgStroke.cpp
        SvgVisibility.cpp
        SvgGradient.cpp
        SvgDefs.cpp
        SvgSvgElement.cpp
        SvgFontSize.cpp
        SvgRasterizer.cpp
        SvgPathInstance.cpp
        SvgPoint.cpp
        SvgPaint.cpp
        SvgCubicBezierCurve.cpp
        SvgEdge.cpp
        SvgPathGenerator.cpp
        SvgRasterizerPaint.cpp
        SvgLinearEquation.cpp
        SvgEllipseEquation.cpp
        SvgShapePolygonizer.cpp
        SvgPolygon.cpp
        SvgBackDrop.cpp
        SvgBlender.cpp
        ../../../include/Elpida/Svg/SvgBlender.hpp
        SvgCompositor.cpp
        ../../../include/Elpida/Svg/SvgCompositor.hpp
        SvgColorConverter.cpp
        ../../../include/Elpida/Svg/SvgColorConverter.hpp
)

target_link_libraries(elpida-svg PRIVATE elpida-xml)