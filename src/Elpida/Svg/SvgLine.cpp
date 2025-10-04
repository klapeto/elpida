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

#include "Elpida/Svg/SvgLine.hpp"

#include "Elpida/Svg/SvgCalculationContext.hpp"
#include "Elpida/Xml/XmlMap.hpp"
#include "Elpida/Svg/SvgPathGenerator.hpp"

namespace Elpida
{
	std::vector<SvgPathInstance> SvgLine::CalculatePaths(const SvgCalculationContext& calculationContext) const
	{
		auto& viewBox = calculationContext.GetViewBox();
		auto x1 = _x1.CalculateValue(calculationContext, viewBox.GetMinX(), viewBox.GetWidth());
		auto y1 = _y1.CalculateValue(calculationContext, viewBox.GetMinY(), viewBox.GetHeight());

		auto x2 = _x2.CalculateValue(calculationContext, viewBox.GetMinX(), viewBox.GetWidth());
		auto y2 = _y2.CalculateValue(calculationContext, viewBox.GetMinY(), viewBox.GetHeight());

		SvgPathGenerator generator;

		generator.MoveTo(SvgPoint(x1, y1));
		generator.LineTo(SvgPoint(x2, y2));
		generator.CommitPath(false);

		return std::move(generator.GetPaths());
	}

	SvgLine::SvgLine(const XmlMap& properties)
	{
		_x1 = SvgLength(properties.GetValue("x1"));
		_y1 = SvgLength(properties.GetValue("y1"));
		_x2 = SvgLength(properties.GetValue("x2"));
		_y2 = SvgLength(properties.GetValue("y2"));
	}
} // Elpida