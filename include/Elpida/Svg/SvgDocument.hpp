//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGDOCUMENT_HPP
#define ELPIDA_SVG_SVGDOCUMENT_HPP

#include "Elpida/Svg/SvgGradient.hpp"
#include "Elpida/Svg/SvgPreserveAspectRatio.hpp"
#include "Elpida/Svg/SvgShape.hpp"
#include "Elpida/Svg/SvgViewBox.hpp"

namespace Elpida
{
	class XmlElement;

	class SvgDocument
	{
	public:
		explicit SvgDocument(const XmlElement& element);
	private:
		std::vector<SvgShape> _shapes;
		std::vector<SvgGradient> _gradients;
		SvgViewBox _viewBox;
		SvgPreserveAspectRatio _preserveAspectRatio;
		SvgCoordinate _width;
		SvgCoordinate _height;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGDOCUMENT_HPP
