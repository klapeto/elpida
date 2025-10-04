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
// Created by klapeto on 25/3/2024.
//

#ifndef ELPIDA_SVGLINE_HPP
#define ELPIDA_SVGLINE_HPP

#include "Elpida/Svg/SvgLength.hpp"
#include "Elpida/Svg/SvgPathInstance.hpp"
#include <vector>

namespace Elpida
{
	class XmlMap;
	class SvgCalculationContext;

	class SvgLine
	{
	public:
		[[nodiscard]]
		std::vector<SvgPathInstance> CalculatePaths(const SvgCalculationContext& calculationContext) const;

		explicit SvgLine(const XmlMap& properties);
	private:
		SvgLength _x1;
		SvgLength _y1;
		SvgLength _x2;
		SvgLength _y2;
	};

} // Elpida

#endif //ELPIDA_SVGLINE_HPP
