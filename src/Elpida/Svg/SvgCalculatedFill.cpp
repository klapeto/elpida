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

#include "Elpida/Svg/SvgCalculatedFill.hpp"
#include "Elpida/Svg/SvgFill.hpp"

namespace Elpida
{
	SvgCalculatedFill::SvgCalculatedFill(const SvgFill& paint, const SvgBounds& elementBounds,
			const SvgDocument& document, const SvgCalculationContext& calculationContext)
			:SvgCalculatedPaint(paint, elementBounds, document, calculationContext), _fillRule(paint.GetFillRule())
	{

	}
} // Elpida