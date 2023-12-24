//
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_SVG_SVGRADIALGRADIENT_HPP
#define ELPIDA_SVG_SVGRADIALGRADIENT_HPP

#include "SvgGradient.hpp"

namespace Elpida
{
	class SvgRadialGradient: public SvgGradient
	{
	public:
		explicit SvgRadialGradient(const XmlElement& element, SvgDefs& defs);
	private:
		SvgLength _cx;
		SvgLength _cy;
		SvgLength _r;
		SvgLength _fx;
		SvgLength _fy;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRADIALGRADIENT_HPP
