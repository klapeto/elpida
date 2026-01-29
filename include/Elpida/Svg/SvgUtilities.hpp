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
// Created by klapeto on 15/4/2024.
//

#ifndef ELPIDA_SVGUTILITIES_HPP
#define ELPIDA_SVGUTILITIES_HPP

#include "SvgConfig.hpp"

namespace Elpida
{
	class SvgUtilities
	{
	public:
		static inline constexpr SvgFloat Pi = 3.141592653589793238462643383279502884;

		template<typename T>
		static constexpr T Lerp(T a, T b, T t) noexcept
		{
			if ((a <= 0 && b >= 0) || (a >= 0 && b <= 0))
			{
				return t * b + (1 - t) * a;
			}

			if (t == 1)
			{
				return b;
			}                        // exact

			// Exact at t=0, monotonic except near t=1,
			// bounded, determinate, and consistent:
			const T x = a + t * (b - a);
			return (t > 1) == (b > a)
				   ? (b < x ? x : b)
				   : (b > x ? x : b);  // monotonic near t=1
		}

		static constexpr SvgFloat Lerp(SvgFloat a, SvgFloat b, SvgFloat t) noexcept
		{
			return Lerp<SvgFloat>(a, b, t);
		}

		SvgUtilities() = delete;
	};
}

#endif //ELPIDA_SVGUTILITIES_HPP
