//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgDocument.hpp"

#include "Elpida/Svg/SvgCoordinate.hpp"
#include "Elpida/Svg/SvgGradient.hpp"
#include "Elpida/Xml/XmlElement.hpp"

#include <string>
#include <string_view>

namespace Elpida
{
	SvgDocument::SvgDocument(const XmlElement& element)
		: SvgSvgElement(element, _defs)
	{
	}
} // Elpida
