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
// Created by klapeto on 21/12/2023.
//

#include "Elpida/Svg/SvgFill.hpp"

#include <Elpida/Svg/SvgNumber.hpp>
#include <Elpida/Xml/XmlMap.hpp>

namespace Elpida
{
	SvgFill::SvgFill(const XmlMap& properties)
		: SvgFill()
	{
		ParseColor(properties.GetValue("fill"));
		ParseOpacity(properties.GetValue("fill-opacity"));
		ParseFillRule(properties.GetValue("fill-rule"));
	}

	void SvgFill::ParseFillRule(const std::string& value)
	{
		if (value.empty()) return;
		CharacterStream stream(value);

		stream.SkipSpace();

		switch (stream.Current())
		{
		case 'e':
			stream.Next();
			if (stream.ConsumeNextChars("venodd"))
			{
				_fillRule = SvgFillRule::EvenOdd;
			}
			break;
		case 'n':
			stream.Next();
			if (stream.ConsumeNextChars("onzero"))
			{
				_fillRule = SvgFillRule::NonZero;
			}
			break;
		default:
			_fillRule = SvgFillRule::NonZero;
			break;
		}
	}
} // Elpida
