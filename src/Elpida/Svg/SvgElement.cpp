//
// Created by klapeto on 20/12/2023.
//

#include "Elpida/Svg/SvgElement.hpp"

#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgPathGenerator.hpp>

#include "Elpida/Svg/SvgDefs.hpp"
#include "Elpida/Svg/SvgNumber.hpp"
#include "Elpida/Svg/SvgStyle.hpp"
#include "Elpida/Svg/SvgVisibility.hpp"

namespace Elpida
{

	SvgElement::SvgElement(const XmlElement& element, SvgDocument& document)
		: _opacity(1.0), _visible(true)
	{
		_properties = element.GetAttributes();

		SvgStyle style(element.GetAttributeValue("style"));
		for (auto& [key, value] : style.GetRules())
		{
			_properties.Set(key, std::move(value));
		}
		ConditionallyAssignProperty<>("id", _id);
		ConditionallyAssignProperty<>("transform", _transform);

		ConditionallyAssignProperty<>("opacity",
			_opacity,
			[](const auto& s)
			{
				double v;
				return SvgNumber::TryParseNumber(s, v) ? v : 1.0;
			});
		ConditionallyAssignProperty<>("display",
			_visible,
			[](const auto& s)
			{
				return static_cast<bool>(SvgVisibility(s));
			});

		_fill = SvgFill(GetProperties());
		_stroke = SvgStroke(GetProperties(), document);

		if (element.GetName() == "path")
		{
			ParseAsPath();
		}
		else if (element.GetName() == "rect")
		{
			ParseAsRectangle(document);
		}

		auto& defs = document._defs;
		for (auto& child : element.GetChildren())
		{
			if (child.GetName() == "defs")
			{
				for (auto& def : child.GetChildren())
				{
					auto& id = def.GetAttributeValue("id");
					if (id.empty()) continue;
					if (def.GetName() == "linearGradient" || def.GetName() == "radialGradient")
					{
						defs.insert({ id,  SvgGradient(def, document) });
					}
				}
				continue;
			}

			_children.emplace_back(child, document);
		}
	}

	void SvgElement::ParseAsRectangle(const SvgDocument& document)
	{
		auto& properties = GetProperties();
		auto& viewBox = document.GetElement().GetViewBox();
		const auto x = SvgLength(properties.GetValue("x")).CalculateActualValue(document,
			viewBox.GetMinX(),
			viewBox.GetWidth());
		const auto y = SvgLength(properties.GetValue("y")).CalculateActualValue(document,
			viewBox.GetMinY(),
			viewBox.GetHeight());

		auto width = SvgLength(properties.GetValue("width")).CalculateActualValue(document, 0, viewBox.GetWidth());
		if (width < 0.0)
		{
			width = 0.0;
		}

		auto height = SvgLength(properties.GetValue("height")).CalculateActualValue(document, 0, viewBox.GetHeight());
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
				rx = fabs(SvgLength(rxStr).CalculateActualValue(document, 0, viewBox.GetWidth()));
			}
		}

		{
			auto& ryStr = properties.GetValue("ry");
			if (!ryStr.empty())
			{
				ry = fabs(SvgLength(ryStr).CalculateActualValue(document, 0, viewBox.GetHeight()));
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

				generator.MoveTo(SvgPoint(x + rx, y));
				generator.LineTo(SvgPoint(x + width - rx, y));
				generator.CubicBezTo(SvgPoint(x + width - rx * (1 - Kappa), y),
					SvgPoint(x + width, y + ry * (1 - Kappa)),
					SvgPoint(x + width, y + ry));
				generator.LineTo(SvgPoint(x + width, y + height - ry));
				generator.CubicBezTo(SvgPoint(x + width, y + height - ry * (1 - Kappa)),
					SvgPoint(x + width - rx * (1 - Kappa), y + height),
					SvgPoint(x + width - rx, y + height));
				generator.LineTo(SvgPoint(x + rx, y + height));
				generator.CubicBezTo(SvgPoint(x + rx * (1 - Kappa), y + height),
					SvgPoint(x, y + height - ry * (1 - Kappa)),
					SvgPoint(x, y + height - ry));
				generator.LineTo(SvgPoint(x, y + ry));
				generator.CubicBezTo(SvgPoint(x, y + ry * (1 - Kappa)),
					SvgPoint(x + rx * (1 - Kappa), y),
					SvgPoint(x + rx, y));
			}

			generator.CommitPath(true);
			_paths = std::move(generator.GetPaths());
		}
	}

	void SvgElement::ParseAsPath()
	{
		SvgPathGenerator generator;
		generator.ParsePathData(_properties.GetValue("d"));
		_paths = std::move(generator.GetPaths());
	}
} // Elpida
