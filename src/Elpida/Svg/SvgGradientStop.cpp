//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgGradientStop.hpp"

#include "Elpida/Svg/SvgElement.hpp"
#include "Elpida/Svg/SvgStyle.hpp"
#include "Elpida/Xml/ParseException.hpp"
#include "Elpida/Xml/XmlElement.hpp"

namespace Elpida
{

	static void SanitizeCoordinate(SvgLength& coordinate)
	{
		switch (coordinate.GetUnits())
		{
		case SvgUnits::Percent:
			if (coordinate.GetValue() > 100.0)
			{
				coordinate = SvgLength(100.0, SvgUnits::Percent);
			}
			break;
		case SvgUnits::Px:
			if (coordinate.GetValue() > 1.0)
			{
				coordinate = SvgLength(1.0, SvgUnits::Px);
			}
			break;
		default:
			coordinate = SvgLength(100.0, SvgUnits::Percent);
			break;
		}

		if (coordinate.GetValue() < 0.0)
		{
			coordinate = SvgLength(0.0, coordinate.GetUnits());
		}
	}

	SvgGradientStop::SvgGradientStop(const XmlElement& element, SvgDocument& document)
		: SvgElement(element, document), _color(SvgColor::Black())
	{
		ConditionallyAssignProperty("stop-color", _color);

		SvgLength opacity(1.0, SvgUnits::Raw);
		if (auto& value = GetProperties().GetValue("stop-opacity"); !value.empty())
		{
			opacity = SvgLength(value);
		}

		SvgLength offset(0.0, SvgUnits::Raw);
		if (auto& value = GetProperties().GetValue("offset"); !value.empty())
		{
			offset = SvgLength(value);
		}

		SanitizeCoordinate(offset);
		SanitizeCoordinate(opacity);

		_offset = offset;
		_opacity = opacity;

	}
} // Elpida