//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgDocument.hpp"

#include "Elpida/Svg/SvgCoordinate.hpp"
#include "Elpida/Svg/SvgGradient.hpp"
#include "Elpida/Xml/ParseException.hpp"
#include "Elpida/Xml/XmlElement.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace Elpida
{
	template <typename T>
	static void ConditionallyAssign(T& value, std::string_view view)
	{
		if (!view.empty())
		{
			value = T(view);
		}
	}

	static void ParseDefs(const XmlElement& element, std::vector<SvgGradient>& gradients)
	{
		for (auto& child : element.GetChildren())
		{
			if (child.GetName() == "linearGradient" || child.GetName() == "radialGradient")
			{
				gradients.emplace_back(child);
			}
		}
	}

	SvgDocument::SvgDocument(const XmlElement& element)
	{
		if (element.GetName() != "svg")
		{
			throw ParseException("Element has invalid tag. It expected '<svg>' and got " + element.GetName());
		}

		if (element.GetAttributeValue("version") != "1.1")
		{
			throw ParseException("Only SVG ver 1.1 is supported");
		}

		_width = SvgCoordinate(element.GetAttributeValue("width"));
		_height = SvgCoordinate(element.GetAttributeValue("height"));
		ConditionallyAssign(_viewBox, element.GetAttributeValue("viewBox"));
		ConditionallyAssign(_preserveAspectRatio, element.GetAttributeValue("preserveAspectRatio"));

		for (auto& child : element.GetChildren())
		{
			if (child.GetName() == "defs")
			{
				ParseDefs(child, _gradients);
			}
		}
	}
} // Elpida
