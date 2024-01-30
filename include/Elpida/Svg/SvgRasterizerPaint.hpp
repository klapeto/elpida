//
// Created by klapeto on 27/1/2024.
//

#ifndef ELPIDA_SVG_SVGRASTERIZERPAINT_HPP
#define ELPIDA_SVG_SVGRASTERIZERPAINT_HPP

#include "SvgColor.hpp"
#include "SvgGradient.hpp"

namespace Elpida
{
	class SvgPoint;

	class SvgRasterizerPaint
	{
	public:
		SvgColor CalculateColor(const SvgPoint& point, const SvgDocument& document) const;

		explicit SvgRasterizerPaint(const SvgPaint& paint, const SvgDocument& document);
	private:
		SvgColor _color;
		const SvgGradient* _gradient;
		const SvgGradient* _stopsGradient;

		void AsGradient(const SvgPaint& paint, const SvgDocument& document);
		void AsColor(const SvgPaint& paint);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZERPAINT_HPP
