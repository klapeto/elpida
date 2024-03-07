//
// Created by klapeto on 27/1/2024.
//

#ifndef ELPIDA_SVG_SVGRASTERIZERPAINT_HPP
#define ELPIDA_SVG_SVGRASTERIZERPAINT_HPP

#include <variant>

#include "SvgColor.hpp"
#include "SvgEllipseEquation.hpp"
#include "SvgGradient.hpp"
#include "SvgLinearEquation.hpp"

namespace Elpida
{
	class SvgPoint;

	class SvgRasterizerPaint final
	{
	public:
		[[nodiscard]]
		SvgColor CalculateColor(const SvgPoint& point) const;

		explicit SvgRasterizerPaint(const SvgPaint& paint, const SvgElement& element, const SvgDocument& document);
		~SvgRasterizerPaint() = default;
	private:
		SvgColor _color;
		const SvgGradient* _gradient;
		const SvgGradient* _stopsGradient;

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

		void AsGradient(const SvgPaint& paint, const SvgElement& element, const SvgDocument& document);
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
		                                        const SvgGradientStop& stopA,
		                                        const SvgLinearEquation& normalA,
		                                        const SvgGradientStop& stopB,
		                                        const SvgLinearEquation& normalB);

		[[nodiscard]]
		static SvgColor InterpolateColor(const SvgGradientStop& stopA, const SvgGradientStop& stopB, double ratio);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZERPAINT_HPP
