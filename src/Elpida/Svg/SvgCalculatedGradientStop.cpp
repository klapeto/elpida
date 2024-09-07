//
// Created by klapeto on 8/3/2024.
//

#include "Elpida/Svg/SvgCalculatedGradientStop.hpp"

namespace Elpida
{
	SvgCalculatedGradientStop::SvgCalculatedGradientStop(const SvgColor& color, SvgFloat offset, SvgFloat opacity)
			:_color(color), _offset(offset), _opacity(opacity)
	{
	}
} // Elpida