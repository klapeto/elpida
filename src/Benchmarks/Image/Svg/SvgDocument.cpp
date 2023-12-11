//
// Created by klapeto on 7/12/2023.
//

#include "SvgDocument.hpp"

#include "Benchmarks/Image/Xml/CharacterStream.hpp"
#include "Benchmarks/Image/Xml/XmlElement.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "SvgCoordinate.hpp"
#include "SvgPreserveAspectRatio.hpp"
#include "SvgViewBox.hpp"
#include <string>
#include <string_view>

namespace Elpida
{
	static const std::string Empty;

	static const std::string& GetAttributeValue(const XmlElement& element, const char* name)
	{
		auto& attributes = element.GetAttributes();
		auto itr = attributes.find(name);
		if (itr != attributes.end())
		{
			return itr->second;
		}

		return Empty;
	}

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
			value += (view[size - i - 1] - '0') * v;
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

		auto integerPart = stream.GetStringViewWhile([](auto c) { return IsNumber(c); });

		if (stream.Current() == '.')
		{
			stream.Next();
		}

		auto floatPart = stream.GetStringViewWhile([](auto c) { return IsNumber(c); });

		double expSign;

		std::string_view expPart;
		auto c = stream.Current();
		if (c == 'e' || c == 'E')
		{
			stream.Next();
			c = stream.Current();
			if (c == '+' || c == '-')
			{
				expSign = c == '+' ? 1.0 : -1.0;
				if (!stream.Next())
				{
					throw ElpidaException("Unexpected EOF. Expected exp number");
				}

				expPart = stream.GetStringViewWhile([](auto c) { return IsNumber(c); });
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

	static SvgUnits ParseUnits(CharacterStream& stream)
	{
		stream.SkipSpace();
		if (stream.Eof()) return SvgUnits::Px;

		auto callback = [&stream](char c, SvgUnits units)
		{
		  if (!stream.Next())
		  {
			  throw ElpidaException("Unexpected EOF. Expected letter after");
		  }
		  if (stream.Current() != c)
		  {
			  throw ElpidaException("Unexpected character: expected '" + std::string() + c);
		  }
		  stream.Next();
		  return units;
		};
		switch (stream.Current())
		{
		case 'p':
			if (!stream.Next())
			{
				throw ElpidaException("Unexpected EOF. Expected letter after 'p'");
			}
			switch (stream.Current())
			{
			case 'x':
				stream.Next();
				return SvgUnits::Px;
			case 't':
				stream.Next();
				return SvgUnits::Pt;
			case 'c':
				stream.Next();
				return SvgUnits::Pc;
			default:
				throw ElpidaException("Unexpected character: expected 'px', 'pt', 'pc'");
			}
		case 'm':
			return callback('m', SvgUnits::Mm);
		case 'c':
			return callback('m', SvgUnits::Cm);
		case 'i':
			return callback('n', SvgUnits::In);
		case '%':
			stream.Next();
			return SvgUnits::Percent;
		case 'e':
			if (!stream.Next())
			{
				throw ElpidaException("Unexpected EOF. Expected letter after 'e'");
			}
			switch (stream.Current())
			{
			case 'm':
				stream.Next();
				return SvgUnits::Em;
			case 'x':
				stream.Next();
				return SvgUnits::Ex;
			default:
				throw ElpidaException("Unexpected character: expected 'em', 'ex'");
			}
		default:
			throw ElpidaException("Unexpected character: expected unit");
		}
	}

	static SvgCoordinate ParseCoordinate(std::string_view view)
	{
		if (view.empty()) return SvgCoordinate{ 0, SvgUnits::Px };

		CharacterStream stream(view);

		return SvgCoordinate{ ParseNumber(stream), ParseUnits(stream) };
	}

	static SvgViewBox ParseViewBox(std::string_view view)
	{
		if (view.empty()) return SvgViewBox{ 0, 0, 0, 0 };

		CharacterStream stream(view);

		auto callback = [](auto c) { return CharacterStream::isspace(c) || c == ',' || c == '%'; };

		auto minX = ParseNumber(stream);
		stream.Skip(callback);

		auto minY = ParseNumber(stream);
		stream.Skip(callback);

		auto width = ParseNumber(stream);
		stream.Skip(callback);

		auto height = ParseNumber(stream);
		stream.Skip(callback);

		return SvgViewBox{ minX, minY, width, height };
	}

	static SvgPreserveAspectRatio ParsePreserveAspectRatio(std::string_view view)
	{
		if (view.empty()) return {};

		CharacterStream stream(view);
		stream.SkipSpace();

		SvgAlignType type = SvgAlignType::Meet;
		SvgAxisAlignType xAlign = SvgAxisAlignType::Mid;
		SvgAxisAlignType yAlign = SvgAxisAlignType::Mid;

		if (stream.Current() == 'n')
		{
			if (!stream.NextCharsAre("none"))
			{
				throw ElpidaException("Unexpected character: expected 'none'");
			}
			return {};
		}

		if (!stream.NextCharsAre("xM"))
		{
			throw ElpidaException("Unexpected character: expected 'xMin' or 'xMax' or 'xMid'");
		}

		auto callback = [&stream](SvgAxisAlignType& align)
		{
		  switch (stream.Current())
		  {
		  case 'i':
			  if (!stream.Next())
			  {
				  throw ElpidaException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
			  }
			  switch (stream.Current())
			  {
			  case 'd':
				  align = SvgAxisAlignType::Mid;
				  break;
			  case 'n':
				  align = SvgAxisAlignType::Min;
				  break;
			  }
			  stream.Next();
			  break;
		  case 'a':
			  if (!stream.Next())
			  {
				  throw ElpidaException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
			  }
			  if (stream.Current() != 'x')
			  {
				  throw ElpidaException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
			  }
			  align = SvgAxisAlignType::Max;
			  stream.Next();
			  break;
		  default:
			  throw ElpidaException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
		  }
		};

		callback(xAlign);

		if (!stream.NextCharsAre("YM"))
		{
			throw ElpidaException("Unexpected character: expected 'YMin' or 'YMax' or 'YMid'");
		}

		callback(yAlign);

		stream.SkipSpace();
		if (!stream.Eof())
		{
			if (stream.Current() == 'm')
			{
				stream.Next();
				if (!stream.NextCharsAre("eet"))
				{
					throw ElpidaException("Unexpected character: expected 'meet'");
				}
				type = SvgAlignType::Meet;
			}
			else if (stream.Current() == 's')
			{
				stream.Next();
				if (!stream.NextCharsAre("lice"))
				{
					throw ElpidaException("Unexpected character: expected 'meet'");
				}
				type = SvgAlignType::Slice;
			}
			else
			{
				throw ElpidaException("Unexpected character: expected 'meet' or 'slice'");
			}
		}

		return SvgPreserveAspectRatio{ type, xAlign, yAlign };
	}

	SvgDocument::SvgDocument(const XmlElement& element)
		: _width(0), _height(0)
	{
		if (element.GetName() != "svg")
		{
			throw ElpidaException("Element has invalid tag. It expected '<svg>' and got " + element.GetName());
		}
		_width = ParseCoordinate(GetAttributeValue(element, "width")).CalculatePixels(0, 0, 0, 0);
		_height = ParseCoordinate(GetAttributeValue(element, "height")).CalculatePixels(0, 0, 0, 0);
		_viewBox = ParseViewBox(GetAttributeValue(element, "viewBox"));
		_preserveAspectRatio = ParsePreserveAspectRatio(GetAttributeValue(element, "preserveAspectRatio"));
	}
} // Elpida