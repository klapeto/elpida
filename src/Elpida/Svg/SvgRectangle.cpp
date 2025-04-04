//
// Created by klapeto on 8/3/2024.
//

#include "Elpida/Svg/SvgRectangle.hpp"

#include "Elpida/Svg/SvgCalculationContext.hpp"
#include "Elpida/Xml/XmlMap.hpp"
#include "Elpida/Svg/SvgPathGenerator.hpp"

namespace Elpida
{
	SvgRectangle::SvgRectangle(const XmlMap& properties)
	{
		_x = SvgLength(properties.GetValue("x"));
		_y = SvgLength(properties.GetValue("y"));
		_width = SvgLength(properties.GetValue("width"));
		_height = SvgLength(properties.GetValue("height"));

		{
			auto& value = properties.GetValue("rx");
			if (!value.empty()) _rX = SvgLength(value);
		}

		{
			auto& value = properties.GetValue("ry");
			if (!value.empty()) _rY = SvgLength(value);
		}

	}

	std::vector<SvgPathInstance> SvgRectangle::CalculatePaths(const SvgCalculationContext& calculationContext) const
	{
		auto& viewBox = calculationContext.GetViewBox();
		const auto x = _x.CalculateValue(calculationContext,
				viewBox.GetMinX(),
				viewBox.GetWidth());
		const auto y = _y.CalculateValue(calculationContext,
				viewBox.GetMinY(),
				viewBox.GetHeight());

		auto width = _width.CalculateValue(calculationContext, viewBox.GetWidth());
		if (width < 0.0)
		{
			width = 0.0;
		}

		auto height = _height.CalculateValue(calculationContext, viewBox.GetHeight());
		if (height < 0.0)
		{
			height = 0.0;
		}

		SvgFloat rx = -1.0; // marks not set
		SvgFloat ry = -1.0;

		if (_rX.has_value())
		{
			rx = std::abs(_rX->CalculateValue(calculationContext, viewBox.GetWidth()));
		}

		if (_rY.has_value())
		{
			ry = std::abs(_rY->CalculateValue(calculationContext, viewBox.GetHeight()));
		}


		if (rx < 0.0 && ry > 0.0) rx = ry;
		if (ry < 0.0 && rx > 0.0) ry = rx;
		if (rx < 0.0) rx = 0.0;
		if (ry < 0.0) ry = 0.0;
		if (rx > width / 2.0) rx = width / SvgFloat(2.0);
		if (ry > height / 2.0) ry = height / SvgFloat(2.0);

		if (width != 0.0 && height != 0.0)
		{
			SvgPathGenerator generator;
			if (rx < 0.00001 || ry < 0.0001)
			{
				generator.MoveTo(SvgPoint(x, y));
				generator.LineTo(SvgPoint(x + width, y));
				generator.LineTo(SvgPoint(x + width, y + height));
				generator.LineTo(SvgPoint(x, y + height));
			}
			else
			{
				// Rounded rectangle
				auto kappa = SvgPathGenerator::Kappa;

				generator.MoveTo(SvgPoint(x + rx, y));
				generator.LineTo(SvgPoint(x + width - rx, y));
				generator.CubicBezTo(SvgPoint(x + width - rx * (1 - kappa), y),
						SvgPoint(x + width, y + ry * (1 - kappa)),
						SvgPoint(x + width, y + ry));
				generator.LineTo(SvgPoint(x + width, y + height - ry));
				generator.CubicBezTo(SvgPoint(x + width, y + height - ry * (1 - kappa)),
						SvgPoint(x + width - rx * (1 - kappa), y + height),
						SvgPoint(x + width - rx, y + height));
				generator.LineTo(SvgPoint(x + rx, y + height));
				generator.CubicBezTo(SvgPoint(x + rx * (1 - kappa), y + height),
						SvgPoint(x, y + height - ry * (1 - kappa)),
						SvgPoint(x, y + height - ry));
				generator.LineTo(SvgPoint(x, y + ry));
				generator.CubicBezTo(SvgPoint(x, y + ry * (1 - kappa)),
						SvgPoint(x + rx * (1 - kappa), y),
						SvgPoint(x + rx, y));
			}

			generator.CommitPath(true);
			return std::move(generator.GetPaths());
		}
		return {};
	}
} // Elpida