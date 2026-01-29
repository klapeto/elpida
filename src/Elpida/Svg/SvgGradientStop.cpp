/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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