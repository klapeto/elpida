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
// Created by klapeto on 11/12/2023.
//

#ifndef ELPIDA_SVG_SVGVIEWBOX_HPP
#define ELPIDA_SVG_SVGVIEWBOX_HPP

#include <string_view>

#include "Elpida/Svg/SvgPoint.hpp"

namespace Elpida
{
	class SvgViewBox
	{
	public:

		[[nodiscard]]
		bool IsValid() const
		{
			return _width > 0.0 && _height > 0.0;
		}

		[[nodiscard]]
		const SvgPoint& GetMin() const
		{
			return _min;
		}

		[[nodiscard]]
		SvgFloat GetMinX() const
		{
			return _min.GetX();
		}

		[[nodiscard]]
		SvgFloat GetMinY() const
		{
			return _min.GetY();
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

		[[nodiscard]]
		SvgFloat GetLength() const
		{
			return _length;
		}

		SvgViewBox();
		SvgViewBox(SvgFloat minX, SvgFloat minY, SvgFloat width, SvgFloat height);
		explicit SvgViewBox(std::string_view view);
	private:
		SvgPoint _min;
		SvgFloat _width;
		SvgFloat _height;
		SvgFloat _length;

		void CalculateLength();
	};

} // Elpida

#endif //ELPIDA_SVG_SVGVIEWBOX_HPP
