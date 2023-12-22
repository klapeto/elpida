//
// Created by klapeto on 21/12/2023.
//

#include "Elpida/Svg/SvgShape.hpp"

#include "Elpida/Svg/SvgNumber.hpp"
#include "Elpida/Svg/SvgVisibility.hpp"

namespace Elpida
{
	SvgShape::SvgShape(const XmlElement& element, SvgDefs& defs)
		: SvgElement(element, defs)
	{
		ConditionallyAssignProperty<>("opacity", _opacity, [](const auto& s) { return SvgNumber::ParseNumber(s); });
		ConditionallyAssignProperty<>("display", _visible, [](const auto& s)
		{
			return static_cast<bool>(SvgVisibility(s));
		});
	}
} // Elpida
