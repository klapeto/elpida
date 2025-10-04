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

#include "Elpida/Svg/SvgStroke.hpp"

#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgLength.hpp>
#include <Elpida/Svg/SvgNumber.hpp>
#include <Elpida/Xml/CharacterStream.hpp>
#include <Elpida/Xml/XmlMap.hpp>

namespace Elpida
{

	SvgStroke::SvgStroke(const XmlMap& properties)
		: SvgStroke()
	{
		ParseColor(properties.GetValue("stroke"));
		ParseOpacity(properties.GetValue("stroke-opacity"));
		ParseWidth(properties.GetValue("stroke-width"));
		ParseDashArray(properties.GetValue("stroke-dasharray"));
		ParseDashOffset(properties.GetValue("stroke-dashoffset"));
		ParseLineCap(properties.GetValue("stroke-linecap"));
		ParseLineJoin(properties.GetValue("stroke-linejoin"));
		ParseMiterLimit(properties.GetValue("stroke-miterlimit"));
	}

	void SvgStroke::ParseWidth(const std::string& value)
	{
		if (value.empty()) return;

		_width = SvgLength(value);
	}

	void SvgStroke::ParseDashArray(const std::string& value)
	{
		if (value.empty()) return;

		CharacterStream stream(value);

		stream.SkipSpace();

		if (stream.ConsumeNextCharsCond("none"))
		{
			return;
		}

		while (!stream.Eof())
		{
			_dashes.push_back(SvgLength(stream.GetStringViewWhile([](auto c)
			{
			  return !CharacterStream::IsSpace(c) && c != ',';
			})));
			stream.Next();
		}
	}

	void SvgStroke::ParseDashOffset(const std::string& dashOffset)
	{
		if (dashOffset.empty()) return;
		_dashOffset = SvgLength(dashOffset);
	}

	void SvgStroke::ParseLineCap(const std::string& value)
	{
		if (value.empty()) return;

		CharacterStream stream(value);
		stream.SkipSpace();
		switch (stream.Current())
		{
		case 'b':
			stream.Next();
			if (stream.ConsumeNextChars("utt"))
			{
				_lineCap = SvgLineCap::Butt;
			}
			break;
		case 'r':
			stream.Next();
			if (stream.ConsumeNextChars("ound"))
			{
				_lineCap = SvgLineCap::Round;
			}
			break;
		case 's':
			stream.Next();
			if (stream.ConsumeNextChars("quare"))
			{
				_lineCap = SvgLineCap::Square;
			}
			break;
		default:
			_lineCap = SvgLineCap::Butt;
			break;
		}
	}

	void SvgStroke::ParseLineJoin(const std::string& value)
	{
		if (value.empty()) return;

		CharacterStream stream(value);

		stream.SkipSpace();

		switch (stream.Current())
		{
		case 'm':
			stream.Next();
			if (stream.ConsumeNextChars("iter"))
			{
				_lineJoin = SvgLineJoin::Miter;
			}
			break;
		case 'r':
			stream.Next();
			if (stream.ConsumeNextChars("ound"))
			{
				_lineJoin = SvgLineJoin::Round;
			}
			break;
		case 'b':
			stream.Next();
			if (stream.ConsumeNextChars("evel"))
			{
				_lineJoin = SvgLineJoin::Bevel;
			}
			break;
		default:
			_lineJoin = SvgLineJoin::Miter;
		}
	}

	void SvgStroke::ParseMiterLimit(const std::string& value)
	{
		if (value.empty()) return;
		SvgNumber::TryParseNumber(value, _miterLimit);
	}
} // Elpida
