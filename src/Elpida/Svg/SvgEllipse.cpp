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

#include "Elpida/Svg/SvgEllipse.hpp"
#include "Elpida/Svg/SvgViewBox.hpp"
#include "Elpida/Svg/SvgCalculationContext.hpp"
#include "Elpida/Svg/SvgPathGenerator.hpp"
#include "Elpida/Xml/XmlMap.hpp"

namespace Elpida
{
	SvgEllipse::SvgEllipse(const XmlMap& properties)
	{
		_cx = SvgLength(properties.GetValue("cx"));
		_cy = SvgLength(properties.GetValue("cy"));
		_rx = SvgLength(properties.GetValue("rx"));
		_ry = SvgLength(properties.GetValue("ry"));
	}

	std::vector<SvgPathInstance> SvgEllipse::CalculatePaths(const SvgCalculationContext& calculationContext) const
	{
		auto& viewBox = calculationContext.GetViewBox();
		auto cx = _cx.CalculateValue(calculationContext, viewBox.GetMinX(), viewBox.GetWidth());
		auto cy = _cy.CalculateValue(calculationContext, viewBox.GetMinY(), viewBox.GetHeight());
		auto rx = std::abs(_rx.CalculateValue(calculationContext, viewBox.GetWidth()));
		auto ry = std::abs(_ry.CalculateValue(calculationContext, viewBox.GetHeight()));

		if (rx > 0.0 && ry > 0.0)
		{
			SvgPathGenerator generator;
			generator.MoveTo(SvgPoint(cx + rx, cy));

			auto kappa = SvgPathGenerator::Kappa;
			generator.CubicBezTo(SvgPoint(cx + rx, cy + ry * kappa),
					SvgPoint(cx + rx * kappa, cy + ry),
					SvgPoint(cx, cy + ry));

			generator.CubicBezTo(SvgPoint(cx - rx * kappa, cy + ry),
					SvgPoint(cx - rx, cy + ry * kappa),
					SvgPoint(cx - rx, cy));

			generator.CubicBezTo(SvgPoint(cx - rx, cy - ry * kappa),
					SvgPoint(cx - rx * kappa, cy - ry),
					SvgPoint(cx, cy - ry));

			generator.CubicBezTo(SvgPoint(cx + rx * kappa, cy - ry),
					SvgPoint(cx + rx, cy - ry * kappa),
					SvgPoint(cx + rx, cy));

			generator.CommitPath(true);
			return std::move(generator.GetPaths());
		}

		return {};
	}
} // Elpida