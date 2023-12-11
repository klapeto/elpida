//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGDOCUMENT_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGDOCUMENT_HPP

#include "Benchmarks/Image/Xml/XmlElement.hpp"
#include "SvgShape.hpp"

namespace Elpida
{
	class XmlElement;

	class SvgDocument
	{
	public:
		explicit SvgDocument(const XmlElement& element);
	private:
		std::vector<SvgShape> _shapes;
		double _width;
		double _height;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGDOCUMENT_HPP
