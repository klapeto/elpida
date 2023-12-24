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
		default: throw ParseException("stop units", "raw or percent");
		}

		if (coordinate.GetValue() < 0.0)
		{
			coordinate = SvgLength(0.0, coordinate.GetUnits());
		}
	}

	SvgGradientStop::SvgGradientStop(const XmlElement& element, SvgDefs& defs)
		: SvgElement(element, defs),_opacity(1.0, SvgUnits::Px)
	{
		ConditionallyAssignProperty("stop-color", _color);
		ConditionallyAssignProperty("stop-opacity", _opacity);
		ConditionallyAssignProperty("offset", _opacity);

		SanitizeCoordinate(_offset);
		SanitizeCoordinate(_opacity);
	}
} // Elpida