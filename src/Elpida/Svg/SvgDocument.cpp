//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgDocument.hpp"

#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Svg/SvgCoordinate.hpp"
#include "Elpida/Svg/SvgGradient.hpp"
#include "Elpida/Xml/XmlElement.hpp"

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace Elpida
{
	static const std::string Empty;

	static const std::string& GetAttributeValue(const XmlElement& element, const char* name)
	{
		auto& attributes = element.GetAttributes();
		auto itr = attributes.find(name);
		if (itr != attributes.end())
		{
			return itr->second;
		}

		return Empty;
	}

	template<typename T>
	static void ConditionallyAssign(T& value, std::string_view view)
	{
		if (!view.empty())
		{
			value = T(view);
		}
	}

	static void ParseDefs(const XmlElement& element, std::vector<SvgGradient>& gradients)
	{
		for (auto& child: element.GetChildren())
		{
			if (child.GetName() == "linearGradient" || child.GetName() == "radialGradient")
			{
				gradients.emplace_back(child);
			}
		}
	}

	SvgDocument::SvgDocument(const XmlElement& element)
			: _width(0), _height(0)
	{
		if (element.GetName() != "svg")
		{
			throw ElpidaException("Element has invalid tag. It expected '<svg>' and got " + element.GetName());
		}

		_width = SvgCoordinate(GetAttributeValue(element, "width")).CalculatePixels(0, 0, 0, 0);
		_height = SvgCoordinate(GetAttributeValue(element, "height")).CalculatePixels(0, 0, 0, 0);
		ConditionallyAssign(_viewBox, GetAttributeValue(element, "viewBox"));
		ConditionallyAssign(_preserveAspectRatio, GetAttributeValue(element, "preserveAspectRatio"));

		for (auto& child: element.GetChildren())
		{
			if (child.GetName() == "defs")
			{
				ParseDefs(child, _gradients);
			}
		}
	}
} // Elpida