//
// Created by klapeto on 20/12/2023.
//

#include "Elpida/Svg/SvgElement.hpp"

#include "Elpida/Svg/SvgDefs.hpp"
#include "Elpida/Svg/SvgElementFactory.hpp"
#include "Elpida/Svg/SvgNumber.hpp"
#include "Elpida/Svg/SvgStyle.hpp"

namespace Elpida
{
	SvgElement::SvgElement(const XmlElement& element, SvgDefs& defs)
	{
		_properties = element.GetAttributes();

		for (SvgStyle style(element.GetAttributeValue("style")); auto& [key, value] : style.GetRules())
		{
			_properties.Set(key, std::move(value));
		}
		ConditionallyAssignProperty<>("id", _id);
		ConditionallyAssignProperty<>("transform", _transform);

		for (auto& child : element.GetChildren())
		{
			if (child.GetName() == "defs")
			{
				for (auto& def : child.GetChildren())
				{
					auto& id = def.GetAttributeValue("id");
					if (id.empty()) continue;
					if (auto defElement = SvgElementFactory::CreateFromXmlElement(def, defs))
					{
						defs.insert({id, std::move(defElement)});
					}
				}
				continue;
			}
			if (auto childElement = SvgElementFactory::CreateFromXmlElement(child, defs))
			{
				_children.push_back(std::move(childElement));
			}
		}
	}

	SvgElement::~SvgElement()
	{
		delete _defs;
	}
} // Elpida
