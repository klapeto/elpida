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
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGCOLOR_HPP
#define ELPIDA_SVG_SVGCOLOR_HPP

#include <string_view>
#include <algorithm>
#include "SvgConfig.hpp"

namespace Elpida
{
	class SvgColor
	{
	public:

		static constexpr SvgFloat MaxValue = 1.0;
		static constexpr SvgFloat HalfValue = MaxValue / 2.0;
		static constexpr SvgFloat QuarterValue = HalfValue / 2.0;
		static constexpr SvgFloat MinValue = 0.0;

		static SvgColor Black()
		{
			return { SvgColor::MinValue, SvgColor::MinValue, SvgColor::MinValue, SvgColor::MaxValue };
		}

		[[nodiscard]]
		SvgFloat R() const
		{
			return _r;
		}

		[[nodiscard]]
		SvgFloat G() const
		{
			return _g;
		}

		[[nodiscard]]
		SvgFloat B() const
		{
			return _b;
		}

		[[nodiscard]]
		SvgFloat A() const
		{
			return _a;
		}

		[[nodiscard]]
		SvgColor WithMultipliedAplha(const SvgFloat alpha) const
		{
			SvgFloat a = _a * alpha;
			a = std::max(MinValue, std::min(MaxValue, a));

			return SvgColor(_r, _g, _b, a);
		}

		SvgColor()
				:_r(MinValue), _g(MinValue), _b(MinValue), _a(MinValue)
		{

		}

		SvgColor(const SvgFloat r, const SvgFloat g, const SvgFloat b, const SvgFloat a)
				:_r(r),
				 _g(g),
				 _b(b),
				 _a(a)
		{
		}

		explicit SvgColor(std::string_view view);

	private:
		SvgFloat _r;
		SvgFloat _g;
		SvgFloat _b;
		SvgFloat _a;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGCOLOR_HPP
