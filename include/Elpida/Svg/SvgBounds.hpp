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

#ifndef ELPIDA_SVG_SVGBOUNDS_HPP
#define ELPIDA_SVG_SVGBOUNDS_HPP

#include "Elpida/Svg/SvgPoint.hpp"
#include "Elpida/Svg/SvgConfig.hpp"

namespace Elpida
{
	class SvgPoint;

	class SvgBounds
	{
	public:
		[[nodiscard]]
		bool Contains(const SvgPoint& point) const;

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
		SvgFloat GetMaxX() const
		{
			return _max.GetX();
		}

		[[nodiscard]]
		SvgFloat GetMaxY() const
		{
			return _max.GetY();
		}

		[[nodiscard]]
		SvgFloat GetWidth() const
		{
			return _max.GetX() - _min.GetX();
		}

		[[nodiscard]]
		SvgFloat GetHeight() const
		{
			return _max.GetY() - _min.GetY();
		}

		void Merge(const SvgBounds& other);

		static SvgBounds CreateMinimum();

		SvgBounds() = default;
		explicit SvgBounds(const SvgPoint& a);
		SvgBounds(const SvgPoint& a, const SvgPoint& b);
		SvgBounds(SvgFloat minX, SvgFloat minY, SvgFloat maxX, SvgFloat maxY);
		~SvgBounds() = default;
	private:
		SvgPoint _min;
		SvgPoint _max;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGBOUNDS_HPP
