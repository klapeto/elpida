//
// Created by klapeto on 9/3/2024.
//

#include "Elpida/Svg/SvgCalculatedViewPort.hpp"

namespace Elpida
{
	SvgCalculatedViewPort::SvgCalculatedViewPort(SvgFloat x, SvgFloat y, SvgFloat width, SvgFloat height)
			:_x(x), _y(y), _width(width), _height(height)
	{
	}

	SvgCalculatedViewPort::SvgCalculatedViewPort()
			:_x(0.0), _y(0.0), _width(0.0), _height(0.0)
	{

	}
} // Elpida