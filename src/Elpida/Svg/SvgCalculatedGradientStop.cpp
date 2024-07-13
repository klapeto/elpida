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

	const SvgColor& SvgCalculatedGradientStop::GetColor() const
	{
		return _color;
	}

	double SvgCalculatedGradientStop::GetOffset() const
	{
		return _offset;
	}

	double SvgCalculatedGradientStop::GetOpacity() const
	{
		return _opacity;
	}
} // Elpida