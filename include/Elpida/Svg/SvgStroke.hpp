//
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_SVG_SVGSTROKE_HPP
#define ELPIDA_SVG_SVGSTROKE_HPP

#include <vector>

#include "SvgPaint.hpp"

namespace Elpida
{
	class SvgDocument;

	enum class SvgLineJoin
	{
		Miter = 0,
		Round = 1,
		Bevel = 2
	};

	enum class SvgLineCap
	{
		Butt = 0,
		Round = 1,
		Square = 2
	};

	class SvgStroke : public SvgPaint
	{
	public:
		SvgStroke(const XmlMap& properties, const SvgDocument& document);
		SvgStroke()
			: SvgPaint(), _lineJoin(SvgLineJoin::Miter), _lineCap(SvgLineCap::Butt), _dashOffset(0), _width(1.0)
		{
		}
	private:
		std::vector<double> _dashes;
		SvgLineJoin _lineJoin;
		SvgLineCap _lineCap;
		double _dashOffset;
		double _width;

		void ParseDashArray(const std::string& value, const SvgDocument& document);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGSTROKE_HPP
