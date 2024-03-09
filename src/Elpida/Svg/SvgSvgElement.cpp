//
// Created by klapeto on 22/12/2023.
//

#include "Elpida/Svg/SvgSvgElement.hpp"

#include <Elpida/Xml/ParseException.hpp>

namespace Elpida
{
	SvgSvgElement::SvgSvgElement(const XmlElement& element, SvgDocument& document)
		: SvgElement(element, document)
	{
		_viewPort = SvgViewPort(element.GetAttributes());
		try
		{
			ConditionallyAssignProperty("viewBox", _viewBox);
		}
		catch (const ParseException&)
		{
			_viewBox = SvgViewBox(0, 0, 0, 0);
		}

		ConditionallyAssignProperty("preserveAspectRatio", _preserveAspectRatio);
	}
} // Elpida
