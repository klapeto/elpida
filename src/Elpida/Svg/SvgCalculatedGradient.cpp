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

#include "Elpida/Svg/SvgCalculatedGradient.hpp"

namespace Elpida
{
	SvgColor SvgCalculatedGradient::InterpolateColor(const SvgCalculatedGradientStop& stopA,
			const SvgCalculatedGradientStop& stopB, SvgFloat ratio)
	{
		auto tR = stopA.GetColor().R() * ratio;
		auto tG = stopA.GetColor().G() * ratio;
		auto tB = stopA.GetColor().B() * ratio;
		auto tA = (stopA.GetColor().A() * stopA.GetOpacity()) * ratio;

		tR += stopB.GetColor().R() * (SvgFloat(1.0) - ratio);
		tG += stopB.GetColor().G() * (SvgFloat(1.0) - ratio);
		tB += stopB.GetColor().B() * (SvgFloat(1.0) - ratio);
		tA += (stopB.GetColor().A() * stopB.GetOpacity()) * (SvgFloat(1.0) - ratio);

		return { tR, tG, tB, tA };
	}

	SvgCalculatedGradient::SvgCalculatedGradient(std::vector<SvgCalculatedGradientStop>&& stops,
			SvgSpreadType spreadType)
			:_stops(std::move(stops)), _spreadType(spreadType)
	{

	}
} // Elpida