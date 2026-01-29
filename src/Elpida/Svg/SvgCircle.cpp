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
// Created by klapeto on 24/3/2024.
//

#include "Elpida/Svg/SvgCircle.hpp"
#include "Elpida/Svg/SvgCalculationContext.hpp"
#include "Elpida/Xml/XmlMap.hpp"
#include "Elpida/Svg/SvgPathGenerator.hpp"

namespace Elpida
{
	std::vector<SvgPathInstance> SvgCircle::CalculatePaths(const SvgCalculationContext& calculationContext) const
	{
		auto& viewBox = calculationContext.GetViewBox();
		auto cx = _cx.CalculateValue(calculationContext, viewBox.GetMinX(), viewBox.GetWidth());
		auto cy = _cy.CalculateValue(calculationContext, viewBox.GetMinY(), viewBox.GetHeight());
		auto r = _r.CalculateValue(calculationContext, viewBox.GetLength());

		if (r > 0.0)
		{
			SvgPathGenerator generator;
			generator.MoveTo(SvgPoint(cx + r, cy));

			auto kappa = SvgPathGenerator::Kappa;
			generator.CubicBezTo(SvgPoint(cx + r, cy + r * kappa),
					SvgPoint(cx + r * kappa, cy + r),
					SvgPoint(cx, cy + r));

			generator.CubicBezTo(SvgPoint(cx - r * kappa, cy + r),
					SvgPoint(cx - r, cy + r * kappa),
					SvgPoint(cx - r, cy));

			generator.CubicBezTo(SvgPoint(cx - r, cy - r * kappa),
					SvgPoint(cx - r * kappa, cy - r),
					SvgPoint(cx, cy - r));

			generator.CubicBezTo(SvgPoint(cx + r * kappa, cy - r),
					SvgPoint(cx + r, cy - r * kappa),
					SvgPoint(cx + r, cy));

			generator.CommitPath(true);
			return std::move(generator.GetPaths());
		}
		return {};
	}

	SvgCircle::SvgCircle(const XmlMap& properties)
	{
		_cx = SvgLength(properties.GetValue("cx"));
		_cy = SvgLength(properties.GetValue("cy"));
		_r = SvgLength(properties.GetValue("r"));
	}
} // Elpida
