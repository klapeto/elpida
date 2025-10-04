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

#ifndef ELPIDA_SVGCALCULATEDFILL_HPP
#define ELPIDA_SVGCALCULATEDFILL_HPP

#include "Elpida/Svg/SvgCalculatedPaint.hpp"
#include "Elpida/Svg/SvgFillRule.hpp"

namespace Elpida
{
	class SvgFill;

	class SvgCalculatedFill : public SvgCalculatedPaint
	{
	public:

		[[nodiscard]]
		SvgFillRule GetFillRule() const
		{
			return _fillRule;
		}

		SvgCalculatedFill(const SvgFill& paint,
				const SvgBounds& elementBounds,
				const SvgDocument& document,
				const SvgCalculationContext& calculationContext);

		SvgCalculatedFill(const SvgCalculatedFill&) = default;
		SvgCalculatedFill& operator=(const SvgCalculatedFill&) = default;
		SvgCalculatedFill(SvgCalculatedFill&&) noexcept = default;
		SvgCalculatedFill& operator=(SvgCalculatedFill&&) noexcept = default;
		~SvgCalculatedFill() override = default;
	private:
		SvgFillRule _fillRule;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDFILL_HPP
