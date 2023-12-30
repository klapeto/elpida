//
// Created by klapeto on 23/12/2023.
//

#include "Elpida/Svg/SvgRect.hpp"

#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgNumber.hpp>

namespace Elpida
{
	SvgRect::SvgRect(const XmlElement& element, SvgDocument& document)
		: SvgPath(element, document)
	{
		auto& properties = GetProperties();
		auto& viewBox = document.GetElement().GetViewBox();
		const auto x = SvgLength(properties.GetValue("x")).CalculateActualValue(document, viewBox.GetMinX(), viewBox.GetWidth());
		const auto y = SvgLength(properties.GetValue("y")).CalculateActualValue(document, viewBox.GetMinY(), viewBox.GetHeight());

		auto width = SvgLength(properties.GetValue("width")).CalculateActualValue(document,0, viewBox.GetWidth());
		if (width < 0.0)
		{
			width = 0.0;
		}

		auto height = SvgLength(properties.GetValue("height")).CalculateActualValue(document,0, viewBox.GetHeight());
		if (height < 0.0)
		{
			height = 0.0;
		}


		double rx = -1.0f; // marks not set
		double ry = -1.0f;

		{
			auto& rxStr = properties.GetValue("rx");
			if (!rxStr.empty())
			{
				rx = fabs(SvgLength(rxStr).CalculateActualValue(document,0, viewBox.GetWidth()));
			}
		}

		{
			auto& ryStr = properties.GetValue("ry");
			if (!ryStr.empty())
			{
				ry = fabs(SvgLength(ryStr).CalculateActualValue(document,0, viewBox.GetHeight()));
			}
		}


		if (rx < 0.0 && ry > 0.0) rx = ry;
		if (ry < 0.0 && rx > 0.0) ry = rx;
		if (rx < 0.0) rx = 0.0;
		if (ry < 0.0) ry = 0.0;
		if (rx > width / 2.0) rx = width / 2.0;
		if (ry > height / 2.0) ry = height / 2.0;

		if (width != 0.0f && height != 0.0f)
		{
			std::vector<SvgPoint> points;

			if (rx < 0.00001 || ry < 0.0001)
			{
				points.reserve(4);
				MoveTo(x, y, points);
				MoveTo(x + width, y, points);
				MoveTo(x + width, y + height, points);
				MoveTo(x, y + height, points);
			}
			else
			{
				points.reserve(1 + 8 * 3);
				// Rounded rectangle
				MoveTo(x + rx, y, points);
				LineTo(x + width - rx, y, points);
				CubicBezTo(x + width - rx * (1 - Kappa), y, x + width, y + ry * (1 - Kappa), x + width, y + ry, points);
				LineTo(x + width, y + height - ry, points);
				CubicBezTo(x + width, y + height - ry * (1 - Kappa), x + width - rx * (1 - Kappa), y + height,
				           x + width - rx, y + height, points);
				LineTo(x + rx, y + height, points);
				CubicBezTo(x + rx * (1 - Kappa), y + height, x, y + height - ry * (1 - Kappa), x, y + height - ry,
				           points);
				LineTo(x, y + ry, points);
				CubicBezTo(x, y + ry * (1 - Kappa), x + rx * (1 - Kappa), y, x + rx, y, points);
			}

			CommitPath(points, true);
		}
	}
} // Elpida
