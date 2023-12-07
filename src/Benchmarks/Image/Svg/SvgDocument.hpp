//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGDOCUMENT_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGDOCUMENT_HPP

#include "Benchmarks/Image/Xml/XmlElement.hpp"
#include "SvgShape.hpp"
namespace Elpida
{

	class SvgDocument
	{
	public:
		SvgDocument(const XmlElement& element);
	private:
		std::vector<SvgShape> _shapes;
		float _width;
		float _height;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGDOCUMENT_HPP
