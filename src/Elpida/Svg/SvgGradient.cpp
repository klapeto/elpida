//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgGradient.hpp"

#include "Elpida/Xml/XmlElement.hpp"
#include "Elpida/Xml/ParseException.hpp"


namespace Elpida
{
	SvgGradient::SvgGradient(const XmlElement& element)
	{
		if (element.GetName() == "linearGradient")
		{
			ParseLinear(element);
		}
		else
		{
			ParseRadial(element);
		}
		ParseBasicData(element);
	}

	void SvgGradient::ParseLinear(const XmlElement& element)
	{
		_type = SvgGradientType::Linear;

		SvgCoordinate x1(0.0, SvgUnits::Percent);
		SvgCoordinate y1(0.0, SvgUnits::Percent);
		SvgCoordinate x2(100.0, SvgUnits::Percent);
		SvgCoordinate y2(0.0, SvgUnits::Percent);

		{
			auto& value = element.GetAttributeValue("x1");
			if (!value.empty())
			{
				x1 = SvgCoordinate(value);
			}
		}

		{
			auto& value = element.GetAttributeValue("y1");
			if (!value.empty())
			{
				y1 = SvgCoordinate(value);
			}
		}

		{
			auto& value = element.GetAttributeValue("x2");
			if (!value.empty())
			{
				x2 = SvgCoordinate(value);
			}
		}

		{
			auto& value = element.GetAttributeValue("y2");
			if (!value.empty())
			{
				y2 = SvgCoordinate(value);
			}
		}

		linear.x1 = x1;
		linear.y1 = y1;
		linear.x2 = x2;
		linear.y2 = y2;
	}

	void SvgGradient::ParseRadial(const XmlElement& element)
	{
		_type = SvgGradientType::Radial;

		SvgCoordinate cx(50.0, SvgUnits::Percent);
		SvgCoordinate cy(50.0, SvgUnits::Percent);
		SvgCoordinate r(50.0, SvgUnits::Percent);
		SvgCoordinate fx(0.0, SvgUnits::Percent);
		SvgCoordinate fy(0.0, SvgUnits::Percent);

		{
			auto& value = element.GetAttributeValue("cx");
			if (!value.empty())
			{
				cx = SvgCoordinate(value);
			}
		}

		{
			auto& value = element.GetAttributeValue("cy");
			if (!value.empty())
			{
				cy = SvgCoordinate(value);
			}
		}

		{
			auto& value = element.GetAttributeValue("r");
			if (!value.empty())
			{
				r = SvgCoordinate(value);
				if (r.GetValue() < 0.0)
				{
					throw ParseException("negative radial gradient radius", "positive radius");
				}
			}
		}

		{
			auto& value = element.GetAttributeValue("fx");
			if (!value.empty())
			{
				fx = SvgCoordinate(value);
			}
			else
			{
				fx = cx;
			}
		}

		{
			auto& value = element.GetAttributeValue("fy");
			if (!value.empty())
			{
				fy = SvgCoordinate(value);
			}
			else
			{
				fy = cx;
			}
		}

		radial.cx = cx;
		radial.cy = cy;
		radial.r = r;
		radial.fx = fx;
		radial.fy = fy;
	}

	void SvgGradient::ParseBasicData(const XmlElement& element)
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
				_stops.emplace_back(child);
			}
		}
		std::sort(_stops.begin(), _stops.end(), [](const SvgGradientStop& a, const SvgGradientStop& b)
		{
			return a.GetOffset().GetValue() < b.GetOffset().GetValue();
		});
		_stops.shrink_to_fit();

		_id = element.GetAttributeValue("id");
		_href = element.GetAttributeValue("xlink:href");
		if (_href.empty())
		{
			_href = element.GetAttributeValue("href");
		}
	}
} // Elpida
