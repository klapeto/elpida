//
// Created by klapeto on 22/12/2023.
//

#include "Elpida/Svg/SvgSvgElement.hpp"

namespace Elpida
{
	SvgSvgElement::SvgSvgElement(const XmlElement& element, SvgDefs& defs)
		: SvgElement(element, defs)
	{
		_width = SvgLength(element.GetAttributeValue("width"));
		_height = SvgLength(element.GetAttributeValue("height"));
		ConditionallyAssignProperty("viewBox", _viewBox);
		ConditionallyAssignProperty("preserveAspectRatio", _preserveAspectRatio);
	}
} // Elpida
