//
// Created by klapeto on 9/3/2024.
//

#ifndef ELPIDA_SVGCALCULATEDVIEWPORT_HPP
#define ELPIDA_SVGCALCULATEDVIEWPORT_HPP

#include "SvgPoint.hpp"
#include "SvgConfig.hpp"

namespace Elpida
{

	class SvgCalculatedViewPort
	{
	public:
		[[nodiscard]]
		SvgFloat GetX() const
		{
			return _x;
		}

		[[nodiscard]]
		SvgFloat GetY() const
		{
			return _y;
		}

		[[nodiscard]]
		SvgFloat GetWidth() const
		{
			return _width;
		}

		[[nodiscard]]
		SvgFloat GetHeight() const
		{
			return _height;
		}

		SvgCalculatedViewPort();
		SvgCalculatedViewPort(SvgFloat x, SvgFloat y, SvgFloat width, SvgFloat height);
	public:
		SvgFloat _x;
		SvgFloat _y;
		SvgFloat _width;
		SvgFloat _height;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDVIEWPORT_HPP
