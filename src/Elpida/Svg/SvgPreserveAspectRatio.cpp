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
// Created by klapeto on 11/12/2023.
//

#include "Elpida/Svg/SvgPreserveAspectRatio.hpp"

#include "Elpida/Xml/CharacterStream.hpp"
#include "Elpida/Xml/ParseException.hpp"

namespace Elpida
{
	SvgPreserveAspectRatio::SvgPreserveAspectRatio(const std::string_view view)
	{
		_type = SvgAlignType::Meet;
		_alignX = SvgAxisAlignType::Mid;
		_alignY = SvgAxisAlignType::Mid;

		CharacterStream stream(view);
		stream.SkipSpace();

		try
		{
			if (stream.Current() == 'n')
			{
				if (!stream.ConsumeNextChars("none"))
				{
					throw ParseException("Unexpected character: expected 'none'");
				}
				_alignX = SvgAxisAlignType::None;
				_alignY = SvgAxisAlignType::None;
				return;
			}

			if (!stream.ConsumeNextChars("xM"))
			{
				throw ParseException("Unexpected character: expected 'xMin' or 'xMax' or 'xMid'");
			}

			auto callback = [&stream](SvgAxisAlignType& align)
			{
				switch (stream.Current())
				{
				case 'i':
					if (!stream.Next())
					{
						throw ParseException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
					}
					switch (stream.Current())
					{
					case 'd':
						align = SvgAxisAlignType::Mid;
						break;
					case 'n':
						align = SvgAxisAlignType::Min;
						break;
					default:
						throw ParseException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
					}
					stream.Next();
					break;
				case 'a':
					if (!stream.Next())
					{
						throw ParseException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
					}
					if (stream.Current() != 'x')
					{
						throw ParseException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
					}
					align = SvgAxisAlignType::Max;
					stream.Next();
					break;
				default:
					throw ParseException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
				}
			};

			callback(_alignX);

			if (!stream.ConsumeNextChars("YM"))
			{
				throw ParseException("Unexpected character: expected 'YMin' or 'YMax' or 'YMid'");
			}

			callback(_alignY);

			stream.SkipSpace();
			if (!stream.Eof())
			{
				if (stream.Current() == 'm')
				{
					stream.Next();
					if (!stream.ConsumeNextChars("eet"))
					{
						throw ParseException("Unexpected character: expected 'meet'");
					}
					_type = SvgAlignType::Meet;
				}
				else if (stream.Current() == 's')
				{
					stream.Next();
					if (!stream.ConsumeNextChars("lice"))
					{
						throw ParseException("Unexpected character: expected 'meet'");
					}
					_type = SvgAlignType::Slice;
				}
				else
				{
					throw ParseException("Unexpected character: expected 'meet' or 'slice'");
				}
			}
		}
		catch (const ParseException&)
		{
			_type = SvgAlignType::Meet;
			_alignX = SvgAxisAlignType::Mid;
			_alignY = SvgAxisAlignType::Mid;
			return;
		}
	}
} // Elpida
