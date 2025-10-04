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

#ifndef ELPIDA_SVGVIEWPORT_HPP
#define ELPIDA_SVGVIEWPORT_HPP

#include "SvgLength.hpp"

namespace Elpida
{
	class XmlMap;

	class SvgViewPort
	{
	public:
		[[nodiscard]]
		const SvgLength& GetX() const
		{
			return _x;
		}

		[[nodiscard]]
		const SvgLength& GetY() const
		{
			return _y;
		}

		[[nodiscard]]
		const SvgLength& GetWidth() const
		{
			return _width;
		}

		[[nodiscard]]
		const SvgLength& GetHeight() const
		{
			return _height;
		}

		SvgViewPort();
		explicit SvgViewPort(const XmlMap& properties);
	private:
		SvgLength _x;
		SvgLength _y;
		SvgLength _width;
		SvgLength _height;
	};

} // Elpida

#endif //ELPIDA_SVGVIEWPORT_HPP
