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

	SvgStroke::SvgStroke(const XmlMap& properties, const SvgDocument& document)
		: SvgPaint(), _lineJoin(SvgLineJoin::Miter), _lineCap(SvgLineCap::Butt), _dashOffset(0), _width(1.0), _miterLimit(0.0)
	{
		ParseColor(properties.GetValue("stroke"));
		ParseOpacity(properties.GetValue("stroke-opacity"));
		ParseWidth(properties.GetValue("stroke-width"), document);
		ParseDashArray(properties.GetValue("stroke-dasharray"), document);
		ParseDashOffset(document, properties.GetValue("stroke-dashoffset"));
		ParseLineCap(properties.GetValue("stroke-linecap"));
		ParseLineJoin(properties.GetValue("stroke-linejoin"));
		ParseMiterLimit(properties.GetValue("stroke-miterlimit"));
	}

	void SvgStroke::ParseWidth(const std::string& value, const SvgDocument& document)
	{
		if (value.empty()) return;

		_width = SvgLength(value).CalculateActualValue(document, 0, document.GetActualLength());
	}

	void SvgStroke::ParseDashArray(const std::string& value, const SvgDocument& document)
	{
		if (value.empty()) return;

		CharacterStream stream(value);

		stream.SkipSpace();

		if (stream.ConsumeNextCharsCond("none"))
		{
			return;
		}

		double sum = 0.0;
		while (!stream.Eof())
		{
			const auto dashLength = SvgLength(stream.GetStringViewWhile([](auto c)
			{
				return !CharacterStream::IsSpace(c) && c != ',';
			})).CalculateActualValue(document, 0, document.GetActualLength());
			sum += dashLength;
			_dashes.push_back(fabs(dashLength));
			stream.Next();
		}

		if (sum <= 1e-6)
		{
			_dashes.clear();
		}
	}

	void SvgStroke::ParseDashOffset(const SvgDocument& document, const std::string& dashOffset)
	{
		if (dashOffset.empty()) return;
		_dashOffset = SvgLength(dashOffset).CalculateActualValue(document, 0, document.GetActualLength());
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
