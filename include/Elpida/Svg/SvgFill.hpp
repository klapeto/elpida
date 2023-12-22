//
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_SVG_SVGFILL_HPP
#define ELPIDA_SVG_SVGFILL_HPP

#include <string>

#include "Elpida/Svg/SvgColor.hpp"

namespace Elpida
{
	enum class SvgFillRule
	{
		NonZero = 0,
		EvenOdd = 1
	};

	class SvgFill
	{
	public:
		SvgFill()
			: _opacity(1.0), _fillRule(SvgFillRule::NonZero)
		{
		}

	private:
		std::string _gradientId;
		SvgColor _color;
		double _opacity;
		SvgFillRule _fillRule;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGFILL_HPP
