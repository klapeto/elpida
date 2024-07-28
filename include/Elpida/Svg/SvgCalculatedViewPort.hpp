//
// Created by klapeto on 9/3/2024.
//

#ifndef ELPIDA_SVGCALCULATEDVIEWPORT_HPP
#define ELPIDA_SVGCALCULATEDVIEWPORT_HPP

#include "SvgPoint.hpp"

namespace Elpida
{

	class SvgCalculatedViewPort
	{
	public:
		[[nodiscard]]
		double GetX() const
		{
			return _x;
		}

		[[nodiscard]]
		double GetY() const
		{
			return _y;
		}

		[[nodiscard]]
		double GetWidth() const
		{
			return _width;
		}

		[[nodiscard]]
		double GetHeight() const
		{
			return _height;
		}

		SvgCalculatedViewPort();
		SvgCalculatedViewPort(double x, double y, double width, double height);
	public:
		double _x;
		double _y;
		double _width;
		double _height;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDVIEWPORT_HPP
