//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgGradient.hpp"

#include <Elpida/Svg/SvgDocument.hpp>

#include "Elpida/Xml/XmlElement.hpp"

namespace Elpida
{
	SvgGradient::SvgGradient(const XmlElement& element, SvgDocument& document)
		: SvgElement(element, document), _type(SvgGradientType::Linear), _spreadType(SvgSpreadType::Pad)
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
				_gradientTransform = SvgTransform(value);
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
				// else
				// {
				// 	throw ParseException(value, "either 'pad' or 'reflect' or 'repeat'");
				// }
			}
		}

		_stops.reserve(element.GetChildren().size());
		for (auto& child : element.GetChildren())
		{
			if (child.GetName() == "stop")
			{
				_stops.emplace_back(child, document);
			}
		}
		_stops.shrink_to_fit();

		_href = element.GetAttributeValue("xlink:href");
		if (_href.empty())
		{
			_href = element.GetAttributeValue("href");
		}

		if (element.GetName() == "linearGradient")
		{
			ParseAsLinear();
		}
		else
		{
			ParseAsRadial();
		}
	}

	void SvgGradient::ParseAsRadial()
	{
		_type = SvgGradientType::Radial;

		_data.radial.cx = SvgLength(50.0, SvgUnits::Percent);
		_data.radial.cy = SvgLength(50.0, SvgUnits::Percent);
		_data.radial.r = SvgLength(50.0, SvgUnits::Percent);
		_data.radial.fx = SvgLength(0.0, SvgUnits::Percent);
		_data.radial.fy = SvgLength(0.0, SvgUnits::Percent);

		ConditionallyAssignProperty("cx", _data.radial.cx);
		ConditionallyAssignProperty("cy", _data.radial.cy);
		ConditionallyAssignProperty("r", _data.radial.r);

		if (_data.radial.r.GetValue() < 0.0)
		{
			_data.radial.r = SvgLength(-_data.radial.r.GetValue(), _data.radial.r.GetUnits());
			//throw ParseException("negative radial gradient radius", "positive radius");
		}

		{
			auto& value = GetProperties().GetValue("fx");
			if (!value.empty())
			{
				_data.radial.fx = SvgLength(value);
			}
			else
			{
				_data.radial.fx = _data.radial.cx;
			}
		}

		{
			auto& value = GetProperties().GetValue("fy");
			if (!value.empty())
			{
				_data.radial.fy = SvgLength(value);
			}
			else
			{
				_data.radial.fy = _data.radial.cy;
			}
		}
	}

	void SvgGradient::ParseAsLinear()
	{
		_data.linear.x1 = SvgLength(0.0, SvgUnits::Percent);
		_data.linear.y1 = SvgLength(0.0, SvgUnits::Percent);
		_data.linear.x2 = SvgLength(100.0, SvgUnits::Percent);
		_data.linear.y2 = SvgLength(100.0, SvgUnits::Percent);

		ConditionallyAssignProperty("x1", _data.linear.x1);
		ConditionallyAssignProperty("y1", _data.linear.y1);
		ConditionallyAssignProperty("x2", _data.linear.x2);
		ConditionallyAssignProperty("y2", _data.linear.y2);
	}
} // Elpida
