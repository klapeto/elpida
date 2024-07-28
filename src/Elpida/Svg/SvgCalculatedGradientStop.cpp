//
// Created by klapeto on 8/3/2024.
//

#include "Elpida/Svg/SvgCalculatedGradientStop.hpp"

namespace Elpida
{
	SvgCalculatedGradientStop::SvgCalculatedGradientStop(const SvgColor& color, double offset, double opacity)
			:_color(color), _offset(offset), _opacity(opacity)
	{
	}
} // Elpida