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
// Created by klapeto on 10/3/2024.
//

#ifndef ELPIDA_SVGCALCULATEDRADIALGRADIENT_HPP
#define ELPIDA_SVGCALCULATEDRADIALGRADIENT_HPP

#include <vector>
#include "Elpida/Svg/SvgCalculatedGradient.hpp"
#include "Elpida/Svg/SvgEllipseEquation.hpp"
#include "Elpida/Svg/SvgCircleEquation.hpp"

namespace Elpida
{
	class SvgGradient;
	class SvgBounds;
	class SvgCalculationContext;

	class SvgCalculatedRadialGradient : public SvgCalculatedGradient
	{
	public:
		void Transform(const SvgTransform& transform);

		[[nodiscard]]
		SvgColor CalculateColor(const SvgPoint& point) const;

		SvgCalculatedRadialGradient(const SvgGradient& gradient,
		                            const SvgBounds& elementBounds,
		                            std::vector<SvgCalculatedGradientStop>&& stops,
		                            const SvgCalculationContext& calculationContext);

	private:
		std::vector<SvgCircleEquation> _circles;
		std::vector<SvgCircleEquation> _inverseCircles;
		SvgTransform _transform;

		[[nodiscard]]
		SvgColor CalculatePad(const SvgPoint& point) const;

		[[nodiscard]]
		SvgColor CalculateRepeat(const SvgPoint& point) const;

		[[nodiscard]]
		static SvgColor CalculateColorFromStops(const SvgPoint& actualPoint,
		                                        const SvgCalculatedGradientStop& stopA,
		                                        const SvgCalculatedGradientStop& stopB,
		                                        const SvgCircleEquation& circleA,
		                                        const SvgCircleEquation& circleB);

		[[nodiscard]]
		SvgColor CalculateReflect(const SvgPoint& point) const;
	};
} // Elpida

#endif //ELPIDA_SVGCALCULATEDRADIALGRADIENT_HPP
