//
// Created by klapeto on 21/12/2023.
//

#include "Elpida/Svg/SvgFill.hpp"

#include <Elpida/Svg/SvgNumber.hpp>
#include <Elpida/Xml/XmlMap.hpp>

namespace Elpida
{
	SvgFill::SvgFill(const XmlMap& properties)
	{
		ParseColor(properties.GetValue("fill"));
		ParseOpacity(properties.GetValue("fill-opacity"));

		auto& fillRuleStr = properties.GetValue("fill-rule");
		if (!fillRuleStr.empty())
		{
			_fillRule = fillRuleStr == "evenodd" ? SvgFillRule::EvenOdd : SvgFillRule::NonZero;
		}
	}
} // Elpida
