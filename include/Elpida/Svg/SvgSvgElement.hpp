//
// Created by klapeto on 22/12/2023.
//

#ifndef ELPIDA_SVG_SVGSVGELEMENT_HPP
#define ELPIDA_SVG_SVGSVGELEMENT_HPP

#include "SvgLength.hpp"
#include "SvgElement.hpp"
#include "SvgPreserveAspectRatio.hpp"
#include "SvgViewBox.hpp"

namespace Elpida
{
	class SvgSvgElement : public SvgElement
	{
	public:
		SvgSvgElement() = default;
		SvgSvgElement(const XmlElement& element, SvgDocument& document);
	private:
		SvgViewBox _viewBox;
		SvgPreserveAspectRatio _preserveAspectRatio;
		SvgLength _width;
		SvgLength _height;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGSVGELEMENT_HPP
