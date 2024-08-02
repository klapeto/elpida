project("Elpida Svg")

add_library(elpida-svg SHARED
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
        SvgCalculatedPaint.cpp
        SvgLinearEquation.cpp
        SvgEllipseEquation.cpp
        SvgShapePolygonizer.cpp
        SvgPolygon.cpp
        SvgBackDrop.cpp
        SvgBlender.cpp
        SvgCompositor.cpp
        SvgColorConverter.cpp
        SvgSuperSampler.cpp
        SvgCalculatedShape.cpp
        SvgRectangle.cpp
        SvgCalculationContext.cpp
        SvgPath.cpp
        SvgCalculatedGradientStop.cpp
        SvgViewPort.cpp
        SvgCalculatedViewPort.cpp
        SvgCalculatedStroke.cpp
        SvgCalculatedFill.cpp
        SvgCalculatedLinearGradient.cpp
        SvgCalculatedRadialGradient.cpp
        SvgCalculatedGradient.cpp
        SvgCircleEquation.cpp
        SvgBlendMode.cpp
        SvgCircle.cpp
        SvgCompositingMode.cpp
        SvgEllipse.cpp
        SvgLine.cpp
        SvgCalculatedDocument.cpp
        SvgDirectRasterizer.cpp
)

target_link_libraries(elpida-svg elpida-core elpida-xml)


install(TARGETS elpida-svg)