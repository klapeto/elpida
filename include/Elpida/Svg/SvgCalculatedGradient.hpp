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

#ifndef ELPIDA_SVGCALCULATEDGRADIENT_HPP
#define ELPIDA_SVGCALCULATEDGRADIENT_HPP

#include <vector>

#include "Elpida/Svg/SvgCalculatedGradientStop.hpp"
#include "Elpida/Svg/SvgColor.hpp"
#include "Elpida/Svg/SvgSpreadType.hpp"

namespace Elpida
{

	class SvgCalculatedGradientStop;

	class SvgCalculatedGradient
	{
	public:
		SvgCalculatedGradient(std::vector<SvgCalculatedGradientStop>&& stops, SvgSpreadType spreadType);
	protected:
		std::vector<SvgCalculatedGradientStop> _stops;
		SvgSpreadType _spreadType;

		static SvgColor InterpolateColor(const SvgCalculatedGradientStop& stopA,
				const SvgCalculatedGradientStop& stopB,
				SvgFloat ratio);
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDGRADIENT_HPP
