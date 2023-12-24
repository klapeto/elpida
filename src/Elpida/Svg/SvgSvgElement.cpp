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
		_width = SvgLength(element.GetAttributeValue("width"));
		_height = SvgLength(element.GetAttributeValue("height"));

		if (_width.GetValue() == 0)
		{
			_width = SvgLength(300, SvgUnits::Px);
		}

		if (_height.GetValue() == 0)
		{
			_width = SvgLength(150, SvgUnits::Px);
		}
		try
		{
			ConditionallyAssignProperty("viewBox", _viewBox);
		}
		catch (const ParseException&)
		{
			_viewBox = SvgViewBox(0, 0, _width.GetValue(), _height.GetValue());
		}

		ConditionallyAssignProperty("preserveAspectRatio", _preserveAspectRatio);
	}
} // Elpida
