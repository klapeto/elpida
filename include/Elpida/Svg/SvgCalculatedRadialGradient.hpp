//
// Created by klapeto on 10/3/2024.
//

#ifndef ELPIDA_SVGCALCULATEDRADIALGRADIENT_HPP
#define ELPIDA_SVGCALCULATEDRADIALGRADIENT_HPP

#include <vector>
#include "Elpida/Svg/SvgEllipseEquation.hpp"
#include "Elpida/Svg/SvgCalculatedGradient.hpp"
#include "Elpida/Svg/SvgSpreadType.hpp"

namespace Elpida
{

	class SvgGradient;
	class SvgBounds;
	class SvgCalculationContext;

	class SvgCalculatedRadialGradient : public SvgCalculatedGradient
	{
	public:
		void Transform(const SvgTransform& transform);

		[[nodiscard]]
		SvgColor CalculateColor(const SvgPoint& point) const;

		SvgCalculatedRadialGradient(const SvgGradient& gradient,
				const SvgBounds& elementBounds,
				std::vector<SvgCalculatedGradientStop>&& stops,
				const SvgCalculationContext& calculationContext);
	private:
		std::vector<SvgEllipseEquation> _equations;

		[[nodiscard]]
		SvgColor CalculatePad(const SvgPoint& point) const;

		[[nodiscard]]
		SvgColor CalculateRepeat(const SvgPoint& point) const;

		[[nodiscard]]
		SvgColor CalculateReflect(const SvgPoint& point) const;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDRADIALGRADIENT_HPP
