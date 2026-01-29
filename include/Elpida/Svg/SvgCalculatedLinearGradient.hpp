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

#ifndef ELPIDA_SVGCALCULATEDLINEARGRADIENT_HPP
#define ELPIDA_SVGCALCULATEDLINEARGRADIENT_HPP

#include <vector>
#include "Elpida/Svg/SvgLinearEquation.hpp"
#include "Elpida/Svg/SvgCalculatedGradientStop.hpp"
#include "Elpida/Svg/SvgCalculatedGradient.hpp"
#include "Elpida/Svg/SvgConfig.hpp"

namespace Elpida
{
	class SvgTransform;
	class SvgBounds;
	class SvgGradient;
	class SvgCalculationContext;

	class SvgCalculatedLinearGradient : public SvgCalculatedGradient
	{
	public:
		void Transform(const SvgTransform& transform);

		[[nodiscard]]
		SvgColor CalculateColor(const SvgPoint& point) const;

		SvgCalculatedLinearGradient(const SvgGradient& gradient,
				const SvgBounds& elementBounds,
				std::vector<SvgCalculatedGradientStop>&& stops,
				const SvgCalculationContext& calculationContext);
	private:
		SvgLinearEquation _equation;
		std::vector<SvgLinearEquation> _stopNormals;
		SvgTransform _transform;
		SvgFloat _length;

		[[nodiscard]]
		SvgColor CalculatePad(const SvgPoint& point) const;

		[[nodiscard]]
		SvgColor CalculateRepeat(const SvgPoint& point) const;

		[[nodiscard]]
		SvgColor CalculateReflect(const SvgPoint& point) const;

		[[nodiscard]]
		SvgColor CalculateColor(const SvgPoint& point,
				const SvgCalculatedGradientStop& stopA,
				const SvgLinearEquation& normalA,
				const SvgCalculatedGradientStop& stopB,
				const SvgLinearEquation& normalB) const;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDLINEARGRADIENT_HPP
