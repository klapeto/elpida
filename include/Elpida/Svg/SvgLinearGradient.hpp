//
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_SVG_SVGLINEARGRADIENT_HPP
#define ELPIDA_SVG_SVGLINEARGRADIENT_HPP

#include "SvgCoordinate.hpp"
#include "SvgGradient.hpp"

namespace Elpida
{
	class SvgLinearGradient: public SvgGradient
	{
	public:
		explicit SvgLinearGradient(const XmlElement& element, SvgDefs& defs);
	private:
		SvgCoordinate _x1;
		SvgCoordinate _y1;
		SvgCoordinate _x2;
		SvgCoordinate _y2;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGLINEARGRADIENT_HPP
