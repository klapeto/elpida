//
// Created by klapeto on 20/12/2023.
//

#ifndef ELPIDA_SVG_SVGELEMENT_HPP
#define ELPIDA_SVG_SVGELEMENT_HPP

#include "Elpida/Xml/XmlElement.hpp"
#include "SvgStyle.hpp"

namespace Elpida
{
	class SvgElement
	{
	public:
		template<typename T>
		static T GetFromAttributeOrStyle(const XmlElement& element, const SvgStyle& style, const std::string& name)
		{
			if (auto& value = element.GetAttributeValue(name); !value.empty())
			{
				return T(value);
			}
			return style.As<T>(name);
		}
	};
} // Elpida

#endif //ELPIDA_SVG_SVGELEMENT_HPP
