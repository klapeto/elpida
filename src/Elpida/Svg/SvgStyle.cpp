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
// Created by klapeto on 20/12/2023.
//

#include "Elpida/Svg/SvgStyle.hpp"

#include "Elpida/Xml/CharacterStream.hpp"
#include "Elpida/Xml/ParseException.hpp"

namespace Elpida
{
	SvgStyle::SvgStyle(const std::string_view view)
	{
		CharacterStream stream(view);
		while (!stream.Eof())
		{
			stream.SkipSpace();
			const auto name = stream.GetStringViewWhile([](auto c){return !CharacterStream::IsSpace(c) && c != ':';});
			stream.SkipSpace();
			if (stream.Current() != ':')
			{
				throw ParseException(stream.Current(), ":");
			}
			if (!stream.Next())
			{
				throw ParseException("Eof", "css rule value");
			}
			stream.SkipSpace();

			const auto firstChar = stream.Index();
			auto lastChar = stream.Index();
			while (!stream.Eof())
			{
				if (stream.Current() == ':')
				{
					throw ParseException(stream.Current(), "css rule value");
				}

				if (stream.Current() == ';')
				{
					break;
				}

				if (!CharacterStream::IsSpace(stream.Current()))
				{
					lastChar = stream.Index();
				}
				stream.Next();
			}
			if (lastChar == firstChar)
			{
				if (const auto c = stream.Char(lastChar); c == ';' || c == ':' || CharacterStream::IsSpace(c))
				{
					throw ParseException("Eof", "css rule value but got empty");
				}
			}
			auto value = stream.GetStringView(firstChar, lastChar);
			stream.SkipSpace();
			if (stream.Current() != ';' && stream.Current() != 0)
			{
				throw ParseException(stream.Current(), "css rule value");
			}
			_rules.Set(std::string(name), std::string(value));
			stream.SkipSpace();
			if (stream.Current() == ';')
			{
				stream.Next();
			}
		}
	}
} // Elpida
