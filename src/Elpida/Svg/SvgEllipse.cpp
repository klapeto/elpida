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