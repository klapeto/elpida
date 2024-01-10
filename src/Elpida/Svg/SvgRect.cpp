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
			std::vector<SvgCubicBezierCurve> curves;

			SvgPoint startPoint;
			if (rx < 0.00001 || ry < 0.0001)
			{
				curves.reserve(3);
				startPoint =  SvgPoint(x, y);
				LineTo(startPoint, SvgPoint(x + width, y), curves);
				LineTo(SvgPoint(x + width, y + height), curves);
				LineTo(SvgPoint(x, y + height), curves);
			}
			else
			{
				curves.reserve(8 * 3);
				// Rounded rectangle

				startPoint = SvgPoint(x + rx, y);
				LineTo(startPoint, SvgPoint(x + width - rx, y), curves);
				CubicBezTo(SvgPoint(x + width - rx * (1 - Kappa), y),SvgPoint( x + width, y + ry * (1 - Kappa)), SvgPoint(x + width, y + ry), curves);
				LineTo(SvgPoint(x + width, y + height - ry), curves);
				CubicBezTo(SvgPoint(x + width, y + height - ry * (1 - Kappa)), SvgPoint(x + width - rx * (1 - Kappa), y + height),
				          SvgPoint( x + width - rx, y + height), curves);
				LineTo(SvgPoint(x + rx, y + height), curves);
				CubicBezTo(SvgPoint(x + rx * (1 - Kappa), y + height), SvgPoint(x, y + height - ry * (1 - Kappa)), SvgPoint(x, y + height - ry),
				           curves);
				LineTo(SvgPoint(x, y + ry), curves);
				CubicBezTo(SvgPoint(x, y + ry * (1 - Kappa)), SvgPoint(x + rx * (1 - Kappa), y), SvgPoint(x + rx, y), curves);
			}

			CommitPath(startPoint, curves, true);
		}
	}
} // Elpida
