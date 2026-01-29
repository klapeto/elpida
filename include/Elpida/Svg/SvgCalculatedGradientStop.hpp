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
// Created by klapeto on 8/3/2024.
//

#ifndef ELPIDA_SVGCALCULATEDGRADIENTSTOP_HPP
#define ELPIDA_SVGCALCULATEDGRADIENTSTOP_HPP

#include "SvgColor.hpp"

namespace Elpida
{
	class SvgCalculatedGradientStop
	{
	public:
		[[nodiscard]]
		const SvgColor& GetColor() const
		{
			return _color;
		}

		[[nodiscard]]
		SvgFloat GetOffset() const
		{
			return _offset;
		}

		[[nodiscard]]
		SvgFloat GetOpacity() const
		{
			return _opacity;
		}

		SvgCalculatedGradientStop(const SvgColor& color, SvgFloat offset, SvgFloat opacity);
	private:
		SvgColor _color;
		SvgFloat _offset;
		SvgFloat _opacity;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDGRADIENTSTOP_HPP
