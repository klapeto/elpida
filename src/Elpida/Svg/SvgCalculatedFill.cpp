//
// Created by klapeto on 9/3/2024.
//

#include "Elpida/Svg/SvgCalculatedFill.hpp"
#include "Elpida/Svg/SvgFill.hpp"

namespace Elpida
{
	SvgCalculatedFill::SvgCalculatedFill(const SvgFill& paint, const SvgBounds& elementBounds,
			const SvgDocument& document, const SvgCalculationContext& calculationContext)
			:SvgCalculatedPaint(paint, elementBounds, document, calculationContext), _fillRule(paint.GetFillRule())
	{

	}

	SvgFillRule SvgCalculatedFill::GetFillRule() const
	{
		return _fillRule;
	}
} // Elpida