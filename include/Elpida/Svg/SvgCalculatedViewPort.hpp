/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
