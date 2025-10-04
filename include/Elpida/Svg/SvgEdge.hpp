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
// Created by klapeto on 22/1/2024.
//

#ifndef ELPIDA_SVG_SVGRASTERIZEREDGE_HPP
#define ELPIDA_SVG_SVGRASTERIZEREDGE_HPP

#include "SvgPoint.hpp"

namespace Elpida
{
	class SvgEdge
	{
	public:
		[[nodiscard]]
		SvgPoint& GetA()
		{
			return _a;
		}

		[[nodiscard]]
		SvgPoint& GetB()
		{
			return _b;
		}

		[[nodiscard]]
		const SvgPoint& GetA() const
		{
			return _a;
		}

		[[nodiscard]]
		const SvgPoint& GetB() const
		{
			return _b;
		}

		[[nodiscard]]
		int GetDirection() const
		{
			return _direction;
		}

		void Transform(const SvgTransform& transform)
		{
			_a.Transform(transform);
			_b.Transform(transform);
		}

		SvgEdge()
				:_direction(0)
		{
		}

		SvgEdge(const SvgPoint& a, const SvgPoint& b, const int direction)
				:_a(a), _b(b), _direction(direction)
		{
		}

		SvgEdge(const SvgEdge&) = default;
		SvgEdge(SvgEdge&&) noexcept = default;
		SvgEdge& operator=(const SvgEdge&) noexcept = default;
		SvgEdge& operator=(SvgEdge&&) noexcept = default;
		~SvgEdge() = default;
	private:
		SvgPoint _a;
		SvgPoint _b;
		int _direction;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZEREDGE_HPP
