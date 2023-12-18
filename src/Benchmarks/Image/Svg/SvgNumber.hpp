//
// Created by klapeto on 17/12/2023.
//

#ifndef SVGNUMBER_HPP
#define SVGNUMBER_HPP

#include <string_view>
#include <cmath>
#include "../Xml/CharacterStream.hpp"
#include <Elpida/Core/ElpidaException.hpp>

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
					throw ElpidaException("Unexpected character: expected digit");
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
					throw ElpidaException("Unexpected value");
				}
			}
			return value;
		}

		static double ParseNumber(CharacterStream& stream)
		{
			stream.SkipSpace();
			double sign;

			switch (stream.Current())
			{
			case '-':
				sign = -1.0;
				if (!stream.Next())
				{
					throw ElpidaException("invalid character, expected number");
				}
				break;
			case '+':
				sign = 1.0;
				if (!stream.Next())
				{
					throw ElpidaException("invalid character, expected number");
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
				throw ElpidaException("No integer or float parts");
			}

			double expSign;

			std::string_view expPart;
			auto c = stream.Current();
			if (c == 'e' || c == 'E')
			{
				c = stream.Char(1);
				if (c == '+' || c == '-')
				{
					stream.Next();
					expSign = c == '+' ? 1.0 : -1.0;
					if (!stream.Next())
					{
						throw ElpidaException("Unexpected EOF. Expected exp number");
					}

					expPart = stream.GetStringViewWhile([](auto c)
					{
						return IsNumber(c);
					});
				}
			}

			double value = StrTol(integerPart);

			value += (double)(StrTol(floatPart)) / pow(10.0, floatPart.length());

			if (!expPart.empty())
			{
				double expValue = StrTol(expPart);
				value *= pow(10.0, expValue * expSign);
			}

			return value * sign;
		}

		static double ParseNumber(const std::string_view& view)
		{
			CharacterStream stream(view);
			return ParseNumber(stream);
		}
	};
} // Elpida

#endif //SVGNUMBER_HPP
