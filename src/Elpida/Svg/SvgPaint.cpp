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
// Created by klapeto on 28/12/2023.
//

#include "Elpida/Svg/SvgPaint.hpp"

#include <Elpida/Svg/SvgNumber.hpp>
#include <Elpida/Xml/CharacterStream.hpp>

namespace Elpida
{
	void SvgPaint::ParseColor(const std::string& value)
	{
		if (value.empty()) return;

		try
		{
			CharacterStream stream(value);

			stream.SkipSpace();
			switch (stream.Current())
			{
			case 'u':
				if (stream.ConsumeNextCharsCond("url("))
				{
					_gradientId = stream.GetStringViewWhile([](auto c)
					{ return c != ')'; });
				}
				else
				{
					_color = SvgColor(stream.GetStringView());
				}
				break;
			case 'n':
				if (stream.ConsumeNextCharsCond("none"))
				{
					_set = false;
					return;
				}
				else
				{
					_color = SvgColor(stream.GetStringView());
				}
				break;
			default:
				_color = SvgColor(stream.GetStringView());
				break;
			}
			_set = true;
		}
		catch (const ParseException&)
		{
			_set = false;
		}
	}

	void SvgPaint::ParseOpacity(const std::string& value)
	{
		if (value.empty()) return;
		SvgNumber::TryParseNumber(value, _opacity);
		if (_opacity > 1.0) _opacity = 1.0;
		if (_opacity < 0.0) _opacity = 0.0;
	}
} // Elpida
