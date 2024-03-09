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
		: SvgPaint(), _lineJoin(SvgLineJoin::Miter), _lineCap(SvgLineCap::Butt), _miterLimit(0.0)
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
