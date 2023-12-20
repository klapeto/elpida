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

	static void SanitizeCoordinate(SvgCoordinate& coordinate)
	{
		switch (coordinate.GetUnits())
		{
		case SvgUnits::Percent:
			if (coordinate.GetValue() > 100.0)
			{
				coordinate = SvgCoordinate(100.0, SvgUnits::Percent);
			}
			break;
		case SvgUnits::Px:
			if (coordinate.GetValue() > 1.0)
			{
				coordinate = SvgCoordinate(1.0, SvgUnits::Px);
			}
			break;
		default: throw ParseException("stop units", "raw or percent");
		}

		if (coordinate.GetValue() < 0.0)
		{
			coordinate = SvgCoordinate(0.0, coordinate.GetUnits());
		}
	}

	SvgGradientStop::SvgGradientStop(const XmlElement& element)
	{
		const SvgStyle style(element.GetAttributeValue("style"));

		_offset = SvgCoordinate(element.GetAttributeValue("offset"));
		_color = SvgElement::GetFromAttributeOrStyle<SvgColor>(element, style, "stop-color");
		_opacity = SvgElement::GetFromAttributeOrStyle<SvgCoordinate>(element, style, "stop-opacity");

		SanitizeCoordinate(_offset);
		SanitizeCoordinate(_opacity);
	}
} // Elpida