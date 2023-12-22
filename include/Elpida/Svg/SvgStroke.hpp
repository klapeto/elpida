//
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_SVG_SVGSTROKE_HPP
#define ELPIDA_SVG_SVGSTROKE_HPP

#include <string>
#include <vector>

#include "Elpida/Svg/SvgColor.hpp"

namespace Elpida
{
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

	class SvgStroke
	{
	public:
		SvgStroke()
			: _lineJoin(SvgLineJoin::Miter), _lineCap(SvgLineCap::Butt), _dashOffset(0), _width(1.0), _opacity(1.0)
		{
		}
	private:
		std::vector<double> _dashes;
		std::string _gradientId;
		SvgColor _color;
		SvgLineJoin _lineJoin;
		SvgLineCap _lineCap;
		double _dashOffset;
		double _width;
		double _opacity;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGSTROKE_HPP
