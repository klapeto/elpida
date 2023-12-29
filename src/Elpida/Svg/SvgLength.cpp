//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgLength.hpp"

#include "Elpida/Svg/SvgNumber.hpp"
#include "Elpida/Svg/SvgDocument.hpp"
#include "Elpida/Xml/CharacterStream.hpp"

namespace Elpida
{
	static SvgUnits ParseUnits(CharacterStream& stream)
	{
		stream.SkipSpace();
		if (stream.Eof()) return SvgUnits::Px;

		auto callback = [&stream](char c, SvgUnits units)
		{
			if (!stream.Next())
			{
				throw ParseException("Unexpected EOF. Expected letter after");
			}
			if (stream.Current() != c)
			{
				throw ParseException("Unexpected character: expected '" + std::string() + c);
			}
			stream.Next();
			return units;
		};
		switch (stream.Current())
		{
		case 'p':
			if (!stream.Next())
			{
				throw ParseException("Unexpected EOF. Expected letter after 'p'");
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
				throw ParseException("Unexpected character: expected 'px', 'pt', 'pc'");
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
				throw ParseException("Unexpected EOF. Expected letter after 'e'");
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
				throw ParseException("Unexpected character: expected 'em', 'ex'");
			}
		default:
			throw ParseException("Unexpected character: expected unit");
		}
	}

	double SvgLength::CalculateActualValue(const SvgDocument& document, const double orig, const double length) const
	{
		switch (_units)
		{
		case SvgUnits::Raw:
		case SvgUnits::Px:
			return _value;
		case SvgUnits::Pt:
			return _value / 72.0f * document.GetDpi();
		case SvgUnits::Pc:
			return _value / 6.0f * document.GetDpi();
		case SvgUnits::Mm:
			return _value / 25.4f * document.GetDpi();
		case SvgUnits::Cm:
			return _value / 2.54f * document.GetDpi();
		case SvgUnits::In:
			return _value * document.GetDpi();
		case SvgUnits::Percent:
			return orig + (_value / 100.0f * length);
		case SvgUnits::Em:
			return _value * document.GetFontSize();
		case SvgUnits::Ex:
			return _value * document.GetFontSize() * 0.52f;
		default:
			return _value;
		}
	}

	SvgLength::SvgLength(const std::string_view view)
	{
		try
		{
			CharacterStream stream(view);
			_value = SvgNumber::ParseNumber(stream);
			_units = ParseUnits(stream);
		}
		catch (const ParseException&)
		{
			_value = 0;
			_units = SvgUnits::Raw;
		}
	}
} // Elpida
