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
		double GetX() const;

		[[nodiscard]]
		double GetY() const;

		[[nodiscard]]
		double GetWidth() const;

		[[nodiscard]]
		double GetHeight() const;

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
