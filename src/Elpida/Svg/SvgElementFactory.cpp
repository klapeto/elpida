//
// Created by klapeto on 21/12/2023.
//

#include "Elpida/Svg/SvgElementFactory.hpp"

#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgGradient.hpp>
#include <Elpida/Svg/SvgLinearGradient.hpp>
#include <Elpida/Svg/SvgRadialGradient.hpp>

namespace Elpida
{
	std::unique_ptr<SvgElement> SvgElementFactory::CreateFromXmlElement(const XmlElement& element, SvgDefs& defs)
	{
		const static std::unordered_map<std::string, std::function<std::unique_ptr<SvgElement>(const XmlElement&, SvgDefs&)>> factories
			= {
				{"linearGradient", [](const auto& e, auto& d) { return std::make_unique<SvgLinearGradient>(e, d); }},
				{"radialGradient", [](const auto& e, auto& d) { return std::make_unique<SvgRadialGradient>(e, d); }},
				{"svg", [](const auto& e, auto& d) { return std::make_unique<SvgSvgElement>(e, d); }},
				{"g", [](const auto& e, auto& d) { return std::make_unique<SvgElement>(e, d); }},
			};

		if (const auto itr = factories.find(element.GetName()); itr != factories.end())
		{
			return itr->second(element, defs);
		}
		return {};
	}
} // Elpida
