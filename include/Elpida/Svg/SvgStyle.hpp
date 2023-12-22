//
// Created by klapeto on 20/12/2023.
//

#ifndef ELPIDA_SVG_SVGSTYLE_HPP
#define ELPIDA_SVG_SVGSTYLE_HPP

#include <string>
#include <Elpida/Xml/XmlMap.hpp>

namespace Elpida
{
	class SvgStyle
	{
	public:
		[[nodiscard]]
		XmlMap& GetRules()
		{
			return _rules;
		}

		[[nodiscard]]
		const XmlMap& GetRules() const
		{
			return _rules;
		}

		SvgStyle() = default;
		explicit SvgStyle(std::string_view view);
	private:
		XmlMap _rules;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGSTYLE_HPP
