//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGDOCUMENT_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGDOCUMENT_HPP

#include "Benchmarks/Image/Svg/SvgViewBox.hpp"
#include "Benchmarks/Image/Xml/XmlElement.hpp"
#include "SvgShape.hpp"
#include "SvgPreserveAspectRatio.hpp"

namespace Elpida
{
	class XmlElement;

	class SvgDocument
	{
	public:
		explicit SvgDocument(const XmlElement& element);
	private:
		std::vector<SvgShape> _shapes;
		SvgViewBox _viewBox;
		SvgPreserveAspectRatio _preserveAspectRatio;
		double _width;
		double _height;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGDOCUMENT_HPP
