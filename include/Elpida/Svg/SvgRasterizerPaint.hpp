//
// Created by klapeto on 27/1/2024.
//

#ifndef ELPIDA_SVG_SVGRASTERIZERPAINT_HPP
#define ELPIDA_SVG_SVGRASTERIZERPAINT_HPP

#include <variant>

#include "SvgColor.hpp"
#include "SvgGradient.hpp"
#include "SvgLinearEquation.hpp"

namespace Elpida
{
	class SvgPoint;

	class SvgRasterizerPaint final
	{
	public:
		[[nodiscard]]
		SvgColor CalculateColor(const SvgPoint& point, const SvgDocument& document) const;

		explicit SvgRasterizerPaint(const SvgPaint& paint, const SvgDocument& document);
		~SvgRasterizerPaint();
	private:
		SvgColor _color;
		const SvgGradient* _gradient;
		const SvgGradient* _stopsGradient;

		struct LinearCache
		{
			SvgLinearEquation equation;
			std::vector<SvgLinearEquation> stopNormals;
			SvgPoint direction;
		};
		std::variant<std::monostate, LinearCache> _gradientCache;

		void AsGradient(const SvgPaint& paint, const SvgDocument& document);
		void AsColor(const SvgPaint& paint);

		[[nodiscard]]
		SvgColor CalculateLinearGradientPad(const SvgPoint& point, const SvgDocument& document) const;
		[[nodiscard]]
		SvgColor CalculateLinearGradientRepeat(const SvgPoint& point, const SvgDocument& document) const;
		[[nodiscard]]
		SvgColor CalculateLinearGradientReflect(const SvgPoint& point, const SvgDocument& document) const;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZERPAINT_HPP
