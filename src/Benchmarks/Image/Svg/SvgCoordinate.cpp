//
// Created by klapeto on 7/12/2023.
//

#include "SvgCoordinate.hpp"

#include "SvgNumber.hpp"

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

	SvgCoordinate::SvgCoordinate(const std::string_view view)
	{
		CharacterStream stream(view);
		_value = SvgNumber::ParseNumber(stream);
		_units = ParseUnits(stream);
	}
} // Elpida
