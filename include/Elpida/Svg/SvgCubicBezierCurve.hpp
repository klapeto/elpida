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
// Created by klapeto on 9/1/2024.
//

#ifndef ELPIDA_SVG_SVGCUBICBEZIERCURVE_HPP
#define ELPIDA_SVG_SVGCUBICBEZIERCURVE_HPP

#include <Elpida/Svg/SvgPoint.hpp>

namespace Elpida
{
	class SvgCubicBezierCurve
	{
	public:
		[[nodiscard]]
		const SvgPoint& GetStartControl() const
		{
			return _startControl;
		}

		[[nodiscard]]
		const SvgPoint& GetEndControl() const
		{
			return _endControl;
		}

		[[nodiscard]]
		const SvgPoint& GetEnd() const
		{
			return _end;
		}

		void Transform(const SvgTransform& transform)
		{
			_startControl.Transform(transform);
			_endControl.Transform(transform);
			_end.Transform(transform);
		}

		SvgCubicBezierCurve(const SvgPoint& startControl, const SvgPoint& endControl, const SvgPoint& end)
			: _startControl(startControl),
			  _endControl(endControl),
			  _end(end)
		{
		}

		SvgCubicBezierCurve(const SvgCubicBezierCurve&) = default;
		SvgCubicBezierCurve(SvgCubicBezierCurve&&) noexcept = default;
		SvgCubicBezierCurve& operator=(const SvgCubicBezierCurve&) = default;
		SvgCubicBezierCurve& operator=(SvgCubicBezierCurve&&) noexcept = default;
	private:
		SvgPoint _startControl;
		SvgPoint _endControl;
		SvgPoint _end;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGCUBICBEZIERCURVE_HPP
