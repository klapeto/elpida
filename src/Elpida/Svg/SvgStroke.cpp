//
// Created by klapeto on 21/12/2023.
//

#include "Elpida/Svg/SvgStroke.hpp"

#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgLength.hpp>
#include <Elpida/Xml/CharacterStream.hpp>
#include <Elpida/Xml/XmlMap.hpp>

namespace Elpida
{
	SvgStroke::SvgStroke(const XmlMap& properties, const SvgDocument& document)
	{
		ParseColor(properties.GetValue("stroke"));
		ParseOpacity(properties.GetValue("stroke-opacity"));

		auto& strokeWidthStr = properties.GetValue("stroke-width");
		if (!strokeWidthStr.empty())
		{
			_width = SvgLength(strokeWidthStr).CalculateActualValue(0, document.GetElement().GetViewBox().GetWidth(), document.GetFontSize(), document.GetDpi());
		}

		auto& strokeDashArrayStr = properties.GetValue("stroke-dasharray");
		if (!strokeDashArrayStr.empty())
		{

		}
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
		else
		{
			while (!stream.Eof())
			{
				auto value = SvgLength(stream.GetStringViewWhile([](auto c){ return !CharacterStream::IsSpace(c) &&c != ',';}))
				.CalculateActualValue();
			}
		}

	}
} // Elpida
