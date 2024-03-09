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

namespace Elpida
{
	class SvgPoint;
	class SvgPaint;
	class SvgBounds;
	class SvgGradient;
	class SvgDocument;
	class SvgCalculationContext;

	class SvgCalculatedPaint final
	{
	public:
		[[nodiscard]]
		SvgColor CalculateColor(const SvgPoint& point) const;

		SvgCalculatedPaint();
		explicit SvgCalculatedPaint(const SvgPaint& paint, const SvgBounds& elementBounds, const SvgDocument& document, const SvgCalculationContext& calculationContext);
		SvgCalculatedPaint(const SvgCalculatedPaint&) = delete;
		SvgCalculatedPaint& operator=(const SvgCalculatedPaint&) noexcept = delete;
		SvgCalculatedPaint(SvgCalculatedPaint&&) noexcept = default;
		SvgCalculatedPaint& operator=(SvgCalculatedPaint&&) noexcept = default;
		~SvgCalculatedPaint() = default;
	private:
		SvgColor _color;
		std::vector<SvgCalculatedGradientStop> _stops;
		SvgGradientType _gradientType;
		SvgSpreadType _spreadType;

		struct LinearCache
		{
			SvgLinearEquation equation;
			std::vector<SvgLinearEquation> stopNormals;
			double length;
		};

		struct RadialCache
		{
			std::vector<SvgEllipseEquation> stopEllipses;
		};
		std::variant<std::monostate, LinearCache, RadialCache> _gradientCache;

		void AsGradient(const SvgPaint& paint, const SvgBounds& elementBounds, const SvgDocument& document, const SvgCalculationContext& calculationContext);
		void AsColor(const SvgPaint& paint);

		[[nodiscard]]
		SvgColor CalculateLinearGradientPad(const SvgPoint& point) const;
		[[nodiscard]]
		SvgColor CalculateLinearGradientRepeat(const SvgPoint& point) const;
		[[nodiscard]]
		SvgColor CalculateLinearGradientReflect(const SvgPoint& point) const;

		[[nodiscard]]
		SvgColor CalculateRadialGradientPad(const SvgPoint& point) const;
		[[nodiscard]]
		SvgColor CalculateRadialGradientRepeat(const SvgPoint& point) const;
		[[nodiscard]]
		SvgColor CalculateRadialGradientReflect(const SvgPoint& point) const;


		[[nodiscard]]
		static SvgColor CalculateColorForLinear(const SvgPoint& point,
		                                        const SvgLinearEquation& gradientEquation,
		                                        const SvgCalculatedGradientStop& stopA,
		                                        const SvgLinearEquation& normalA,
		                                        const SvgCalculatedGradientStop& stopB,
		                                        const SvgLinearEquation& normalB);

		[[nodiscard]]
		static SvgColor InterpolateColor(const SvgCalculatedGradientStop& stopA, const SvgCalculatedGradientStop& stopB, double ratio);
		static void GetGradients(const SvgDocument& document, const SvgPaint& paint, const SvgGradient** gradient, const SvgGradient** gradientStops);
		void CalculateStops(const SvgGradient& gradientStops, const SvgCalculationContext& calculationContext);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZERPAINT_HPP
