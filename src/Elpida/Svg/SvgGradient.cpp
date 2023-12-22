//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgGradient.hpp"

#include "Elpida/Xml/ParseException.hpp"
#include "Elpida/Xml/XmlElement.hpp"

namespace Elpida
{
	SvgGradient::SvgGradient(const XmlElement& element, SvgDefs& defs)
		: SvgElement(element, defs)
	{
		{
			auto& value = element.GetAttributeValue("gradientUnits");
			if (!value.empty())
			{
				_units = value == "objectBoundingBox" ? SvgGradientUnits::Object : SvgGradientUnits::User;
			}
		}

		{
			auto& value = element.GetAttributeValue("gradientTransform");
			if (!value.empty())
			{
				_transform = SvgTransform(value);
			}
		}

		{
			auto& value = element.GetAttributeValue("spreadMethod");
			if (!value.empty())
			{
				if (value == "pad")
				{
					_spreadType = SvgSpreadType::Pad;
				}
				else if (value == "reflect")
				{
					_spreadType = SvgSpreadType::Reflect;
				}
				else if (value == "repeat")
				{
					_spreadType = SvgSpreadType::Repeat;
				}
				else
				{
					throw ParseException(value, "either 'pad' or 'reflect' or 'repeat'");
				}
			}
		}

		_stops.reserve(element.GetChildren().size());
		for (auto& child : element.GetChildren())
		{
			if (child.GetName() == "stop")
			{
				_stops.emplace_back(child, defs);
			}
		}
		_stops.shrink_to_fit();

		_href = element.GetAttributeValue("xlink:href");
		if (_href.empty())
		{
			_href = element.GetAttributeValue("href");
		}
	}
} // Elpida
