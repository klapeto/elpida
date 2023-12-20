//
// Created by klapeto on 20/12/2023.
//

#ifndef ELPIDA_SVG_SVGELEMENT_HPP
#define ELPIDA_SVG_SVGELEMENT_HPP

#include "SvgStyle.hpp"
#include "Elpida/Xml/XmlElement.hpp"

namespace Elpida
{
	class SvgElement
	{
	public:
		template<typename T>
		static void GetFromAttributeOrStyle(const XmlElement& element, const SvgStyle& style, const std::string& name, T& targetValue)
		{
			if (auto& value = element.GetAttributeValue(name); !value.empty())
			{
				targetValue = T(value);
			}
			auto& rules = style.GetRules();
			if (const auto itr = rules.find(name); itr != rules.end())
			{
				targetValue = T(itr->second);
			}
		}
	};
} // Elpida

#endif //ELPIDA_SVG_SVGELEMENT_HPP
