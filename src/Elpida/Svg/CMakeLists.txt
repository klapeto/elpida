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
        Shapes/SvgCubicBezierCurve.cpp
        SvgRasterizerEdge.cpp
        SvgRasterizerShape.cpp
        SvgPathGenerator.cpp
)

target_link_libraries(elpida-svg PRIVATE elpida-xml)