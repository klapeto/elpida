//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGDOCUMENT_HPP
#define ELPIDA_SVG_SVGDOCUMENT_HPP

#include "SvgDefs.hpp"
#include "SvgSvgElement.hpp"

namespace Elpida
{
	class XmlElement;

	class SvgDocument: public SvgSvgElement
	{
	public:
		explicit SvgDocument(const XmlElement& element);
	private:
		SvgDefs _defs;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGDOCUMENT_HPP
