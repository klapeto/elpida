//
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_SVG_SVGELEMENTFACTORY_HPP
#define ELPIDA_SVG_SVGELEMENTFACTORY_HPP
#include <memory>

#include "SvgElement.hpp"

namespace Elpida
{
	class SvgElementFactory
	{
	public:
		static std::unique_ptr<SvgElement> CreateFromXmlElement(const XmlElement& element, SvgDocument& document);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGELEMENTFACTORY_HPP
