//
// Created by klapeto on 21/12/2023.
//

#include "Elpida/Svg/SvgFill.hpp"

#include <Elpida/Svg/SvgNumber.hpp>
#include <Elpida/Xml/XmlMap.hpp>

namespace Elpida
{
	SvgFill::SvgFill(const XmlMap& properties)
		: _fillRule(SvgFillRule::NonZero)
	{
		ParseColor(properties.GetValue("fill"));
		ParseOpacity(properties.GetValue("fill-opacity"));
		ParseFillRule(properties.GetValue("fill-rule"));
	}

	void SvgFill::ParseFillRule(const std::string& value)
	{
		if (value.empty()) return;
		CharacterStream stream(value);

		stream.SkipSpace();

		switch (stream.Current())
		{
		case 'e':
			stream.Next();
			if (stream.ConsumeNextChars("venodd"))
			{
				_fillRule = SvgFillRule::EvenOdd;
			}
			break;
		case 'n':
			stream.Next();
			if (stream.ConsumeNextChars("onzero"))
			{
				_fillRule = SvgFillRule::NonZero;
			}
			break;
		default:
			_fillRule = SvgFillRule::NonZero;
			break;
		}
	}
} // Elpida
