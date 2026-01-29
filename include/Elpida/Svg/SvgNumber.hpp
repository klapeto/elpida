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
// Created by klapeto on 17/12/2023.
//

#ifndef ELPIDA_SVG_SVGNUMBER_HPP
#define ELPIDA_SVG_SVGNUMBER_HPP

#include <cmath>
#include <string_view>

#include "Elpida/Svg/SvgConfig.hpp"
#include <Elpida/Xml/CharacterStream.hpp>
#include <Elpida/Xml/ParseException.hpp>

namespace Elpida
{
	class SvgNumber
	{
	public:

		static bool IsNumber(const char c)
		{
			return (unsigned char)(c - '0') < 10;
		}

		static unsigned long StrTol(const std::string_view& view)
		{
			unsigned long value = 0;
			const std::size_t size = view.length();
			for (std::size_t i = 0, v = 1; i < size; ++i, v *= 10)
			{
				if (const auto c = view[size - i - 1]; IsNumber(c))
				{
					value += (view[size - i - 1] - '0') * v;
				}
				else
				{
					throw ParseException("Unexpected character: expected digit");
				}
			}
			return value;
		}

		static unsigned long StrTol16(const std::string_view& view)
		{
			unsigned long value = 0;
			const std::size_t size = view.length();
			for (std::size_t i = 0, v = 1; i < size; ++i, v *= 16)
			{
				if (const auto c = view[size - i - 1]; c >= '0' && c <= '9')
				{
					value += (c - '0') * v;
				}
				else if (c >= 'A' && c <= 'F')
				{
					value += (c - 'A' + 10) * v;
				}
				else if (c >= 'a' && c <= 'f')
				{
					value += (c - 'a' + 10) * v;
				}
				else
				{
					throw ParseException("Unexpected value");
				}
			}
			return value;
		}

		static SvgFloat ParseNumber(CharacterStream& stream)
		{
			stream.SkipSpace();
			SvgFloat sign;

			switch (stream.Current())
			{
			case '-':
				sign = -1.0;
				if (!stream.Next())
				{
					throw ParseException("invalid character, expected number");
				}
				break;
			case '+':
				sign = 1.0;
				if (!stream.Next())
				{
					throw ParseException("invalid character, expected number");
				}
				break;
			default:
				sign = 1.0;
				break;
			}

			const auto integerPart = stream.GetStringViewWhile([](auto c) { return IsNumber(c); });

			if (stream.Current() == '.')
			{
				stream.Next();
			}

			const auto floatPart = stream.GetStringViewWhile([](auto c) { return IsNumber(c); });

			if (integerPart.empty() && floatPart.empty())
			{
				throw ParseException("No integer or float parts");
			}

			SvgFloat expSign = 1.0;

			std::string_view expPart;
			auto c = stream.Current();
			if (c == 'e' || c == 'E')
			{
				c = stream.FutureChar(1);
				if (c == '+' || c == '-')
				{
					stream.Next();
					expSign = c == '+' ? 1.0 : -1.0;
					if (!stream.Next())
					{
						throw ParseException("Unexpected EOF. Expected exp number");
					}

					expPart = stream.GetStringViewWhile([](auto c)
					{
						return IsNumber(c);
					});
				}
			}

			SvgFloat value = StrTol(integerPart);

			value += (SvgFloat)(StrTol(floatPart)) / std::pow(SvgFloat(10.0), floatPart.length());

			if (!expPart.empty())
			{
				SvgFloat expValue = StrTol(expPart);
				value *= std::pow(SvgFloat(10.0), expValue * expSign);
			}

			return value * sign;
		}

		static SvgFloat ParseNumber(const std::string_view& view)
		{
			CharacterStream stream(view);
			return ParseNumber(stream);
		}

		static bool TryParseNumber(const std::string_view& view, SvgFloat& value)
		{
			try
			{
				CharacterStream stream(view);
				value = ParseNumber(stream);
				return true;
			}
			catch (const ParseException&)
			{
				return false;
			}
		}

		operator SvgFloat() const
		{
			return _value;
		}

		explicit SvgNumber(const std::string_view view)
		{
			_value = ParseNumber(view);
		}

	private:
		SvgFloat _value;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGNUMBER_HPP
