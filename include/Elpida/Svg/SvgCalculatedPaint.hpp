//
// Created by klapeto on 27/1/2024.
//

#ifndef ELPIDA_SVG_SVGRASTERIZERPAINT_HPP
#define ELPIDA_SVG_SVGRASTERIZERPAINT_HPP

#include <variant>
#include <vector>

#include "Elpida/Svg/SvgColor.hpp"
#include "Elpida/Svg/SvgEllipseEquation.hpp"
#include "Elpida/Svg/SvgLinearEquation.hpp"
#include "Elpida/Svg/SvgCalculatedGradientStop.hpp"
#include "Elpida/Svg/SvgGradientType.hpp"
#include "Elpida/Svg/SvgSpreadType.hpp"
#include "SvgCalculatedLinearGradient.hpp"
#include "SvgCalculatedRadialGradient.hpp"
#include "SvgConfig.hpp"

namespace Elpida
{
	class SvgPoint;
	class SvgPaint;
	class SvgBounds;
	class SvgGradient;
	class SvgDocument;
	class SvgCalculationContext;

	class SvgCalculatedPaint
	{
	public:
		[[nodiscard]]
		SvgColor CalculateColor(const SvgPoint& point) const;

		virtual void Transform(const SvgTransform& transform);

		[[nodiscard]]
		SvgFloat Opacity() const
		{
			return _opacity;
		}

		SvgCalculatedPaint();
		explicit SvgCalculatedPaint(const SvgPaint& paint,
				const SvgBounds& elementBounds,
				const SvgDocument& document,
				const SvgCalculationContext& calculationContext);
		SvgCalculatedPaint(const SvgCalculatedPaint&) = default;
		SvgCalculatedPaint& operator=(const SvgCalculatedPaint&) = default;
		SvgCalculatedPaint(SvgCalculatedPaint&&) noexcept = default;
		SvgCalculatedPaint& operator=(SvgCalculatedPaint&&) noexcept = default;
		virtual ~SvgCalculatedPaint() = default;
	private:
		std::variant<SvgColor, SvgCalculatedLinearGradient, SvgCalculatedRadialGradient> _state;
		SvgGradientType _gradientType;
		bool _color;
		SvgFloat _opacity;

		void AsGradient(const SvgPaint& paint, const SvgBounds& elementBounds, const SvgDocument& document,
				const SvgCalculationContext& calculationContext);
		void AsColor(const SvgPaint& paint);

		static void GetGradients(const SvgDocument& document, const SvgPaint& paint, const SvgGradient** gradient,
				const SvgGradient** gradientStops);
		static std::vector<SvgCalculatedGradientStop> CalculateStops(const SvgGradient& gradientStops, const SvgCalculationContext& calculationContext);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZERPAINT_HPP
