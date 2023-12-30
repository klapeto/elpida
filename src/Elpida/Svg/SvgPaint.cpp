//
// Created by klapeto on 28/12/2023.
//

#include "Elpida/Svg/SvgPaint.hpp"

#include <Elpida/Svg/SvgNumber.hpp>
#include <Elpida/Xml/CharacterStream.hpp>

namespace Elpida
{
	void SvgPaint::ParseColor(const std::string& value)
	{
		if (value.empty()) return;
		CharacterStream stream(value);

		stream.SkipSpace();
		switch (stream.Current())
		{
		case 'u':
			if (stream.ConsumeNextCharsCond("url("))
			{
				_gradientId = stream.GetStringViewWhile([](auto c) { return c != ')'; });
			}
			else
			{
				_color = SvgColor(stream.GetStringView());
			}
			break;
		case 'n':
			if (stream.ConsumeNextCharsCond("none"))
			{
				return;
			}
			else
			{
				_color = SvgColor(stream.GetStringView());
			}
			break;
		default:
			_color = SvgColor(stream.GetStringView());
			break;
		}
	}

	void SvgPaint::ParseOpacity(const std::string& value)
	{
		if (value.empty()) return;
		SvgNumber::TryParseNumber(value, _opacity);
		if (_opacity > 1.0) _opacity = 1.0;
		if (_opacity < 0.0) _opacity = 0.0;
	}
} // Elpida
