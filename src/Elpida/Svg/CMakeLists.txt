project("Elpida Svg")

add_library(elpida-svg
        SvgBounds.cpp
        SvgColor.cpp
        SvgCoordinate.cpp
        SvgDocument.cpp
        SvgGradient.cpp
        SvgGradientStop.cpp
        SvgNumber.cpp
        SvgPaint.cpp
        SvgPath.cpp
        SvgPreserveAspectRatio.cpp
        SvgShape.cpp
        SvgTransform.cpp
        SvgViewBox.cpp
        SvgStyle.cpp
        SvgElement.cpp
)

target_link_libraries(elpida-svg PRIVATE elpida-xml)