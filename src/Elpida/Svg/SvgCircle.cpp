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
		auto cy = _cy.CalculateValue(calculationContext, viewBox.GetMinY(),viewBox.GetHeight());
		auto r = _r.CalculateValue(calculationContext, viewBox.GetLength());

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

	SvgCircle::SvgCircle(const XmlMap& properties)
	{
		_cx = SvgLength(properties.GetValue("cx"));
		_cy = SvgLength(properties.GetValue("cy"));
		_r = SvgLength(properties.GetValue("r"));
	}
} // Elpida
