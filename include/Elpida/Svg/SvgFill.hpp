//
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_SVG_SVGFILL_HPP
#define ELPIDA_SVG_SVGFILL_HPP

#include "SvgPaint.hpp"

namespace Elpida
{
	enum class SvgFillRule
	{
		NonZero = 0,
		EvenOdd = 1
	};

	class SvgFill : public SvgPaint
	{
	public:
		SvgFill(const XmlMap& properties);
		SvgFill()
			: SvgPaint(), _fillRule(SvgFillRule::NonZero)
		{
		}
	private:
		SvgFillRule _fillRule;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGFILL_HPP
