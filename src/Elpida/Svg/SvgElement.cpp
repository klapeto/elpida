//
// Created by klapeto on 20/12/2023.
//

#include "Elpida/Svg/SvgElement.hpp"

#include <Elpida/Svg/SvgDocument.hpp>

#include "Elpida/Svg/SvgDefs.hpp"
#include "Elpida/Svg/SvgElementFactory.hpp"
#include "Elpida/Svg/SvgNumber.hpp"
#include "Elpida/Svg/SvgStyle.hpp"

namespace Elpida
{


	SvgElement::SvgElement(const XmlElement& element, SvgDocument& document)
	{
		_properties = element.GetAttributes();

		SvgStyle style(element.GetAttributeValue("style"));
		for (auto& [key, value] : style.GetRules())
		{
			_properties.Set(key, std::move(value));
		}
		ConditionallyAssignProperty<>("id", _id);
		ConditionallyAssignProperty<>("transform", _transform);

		StackedValue fontSize(document._fontSizes, "font-size", [&](auto& s) -> std::optional<double>
		{
			return {SvgLength(s).CalculateActualValue(document, 0, document.GetActualLength())};
		});

		auto& defs = document._defs;
		for (auto& child : element.GetChildren())
		{
			if (child.GetName() == "defs")
			{
				for (auto& def : child.GetChildren())
				{
					auto& id = def.GetAttributeValue("id");
					if (id.empty()) continue;
					if (auto defElement = SvgElementFactory::CreateFromXmlElement(def, document))
					{
						defs.insert({id, std::move(defElement)});
					}
				}
				continue;
			}
			if (auto childElement = SvgElementFactory::CreateFromXmlElement(child, document))
			{
				_children.push_back(std::move(childElement));
			}
		}
	}
} // Elpida
