//
// Created by klapeto on 10/3/2024.
//

#ifndef ELPIDA_SVGCALCULATEDLINEARGRADIENT_HPP
#define ELPIDA_SVGCALCULATEDLINEARGRADIENT_HPP

#include <vector>
#include "Elpida/Svg/SvgLinearEquation.hpp"
#include "Elpida/Svg/SvgCalculatedGradientStop.hpp"
#include "Elpida/Svg/SvgCalculatedGradient.hpp"

namespace Elpida
{
	class SvgTransform;
	class SvgBounds;
	class SvgGradient;
	class SvgCalculationContext;

	class SvgCalculatedLinearGradient : public SvgCalculatedGradient
	{
	public:
		void Transform(const SvgTransform& transform);

		[[nodiscard]]
		SvgColor CalculateColor(const SvgPoint& point) const;

		SvgCalculatedLinearGradient(const SvgGradient& gradient,
				const SvgBounds& elementBounds,
				std::vector<SvgCalculatedGradientStop>&& stops,
				const SvgCalculationContext& calculationContext);
	private:
		SvgLinearEquation _equation;
		std::vector<SvgLinearEquation> _stopNormals;
		SvgTransform _transform;
		double _length;

		[[nodiscard]]
		SvgColor CalculatePad(const SvgPoint& point) const;

		[[nodiscard]]
		SvgColor CalculateRepeat(const SvgPoint& point) const;

		[[nodiscard]]
		SvgColor CalculateReflect(const SvgPoint& point) const;

		[[nodiscard]]
		SvgColor CalculateColor(const SvgPoint& point,
				const SvgCalculatedGradientStop& stopA,
				const SvgLinearEquation& normalA,
				const SvgCalculatedGradientStop& stopB,
				const SvgLinearEquation& normalB) const;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDLINEARGRADIENT_HPP
