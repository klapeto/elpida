//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgDocument.hpp"

#include "Elpida/Xml/XmlElement.hpp"

namespace Elpida
{
	SvgDocument::SvgDocument(const XmlElement& element)
	{
		_element = SvgSvgElement(element, *this);
	}
} // Elpida
