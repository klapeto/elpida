//
// Created by klapeto on 9/3/2024.
//

#ifndef ELPIDA_SVGCALCULATEDFILL_HPP
#define ELPIDA_SVGCALCULATEDFILL_HPP

#include "Elpida/Svg/SvgCalculatedPaint.hpp"
#include "Elpida/Svg/SvgFillRule.hpp"

namespace Elpida
{
	class SvgFill;

	class SvgCalculatedFill : public SvgCalculatedPaint
	{
	public:

		[[nodiscard]]
		SvgFillRule GetFillRule() const
		{
			return _fillRule;
		}

		SvgCalculatedFill(const SvgFill& paint,
				const SvgBounds& elementBounds,
				const SvgDocument& document,
				const SvgCalculationContext& calculationContext);

		SvgCalculatedFill(const SvgCalculatedFill&) = default;
		SvgCalculatedFill& operator=(const SvgCalculatedFill&) = default;
		SvgCalculatedFill(SvgCalculatedFill&&) noexcept = default;
		SvgCalculatedFill& operator=(SvgCalculatedFill&&) noexcept = default;
		~SvgCalculatedFill() override = default;
	private:
		SvgFillRule _fillRule;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDFILL_HPP
