//
// Created by klapeto on 9/3/2024.
//

#include "Elpida/Svg/SvgCalculatedViewPort.hpp"

namespace Elpida
{
	SvgCalculatedViewPort::SvgCalculatedViewPort(double x, double y, double width, double height)
			:_x(x), _y(y), _width(width), _height(height)
	{
	}

	double SvgCalculatedViewPort::GetX() const
	{
		return _x;
	}

	double SvgCalculatedViewPort::GetY() const
	{
		return _y;
	}

	double SvgCalculatedViewPort::GetWidth() const
	{
		return _width;
	}

	double SvgCalculatedViewPort::GetHeight() const
	{
		return _height;
	}
} // Elpida