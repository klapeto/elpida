//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGDOCUMENT_HPP
#define ELPIDA_SVG_SVGDOCUMENT_HPP

#include <stack>

#include "SvgDefs.hpp"
#include "SvgSvgElement.hpp"

namespace Elpida
{
	class XmlElement;

	class SvgDocument
	{
	public:
		[[nodiscard]]
		const SvgDefs& GetDefs() const
		{
			return _defs;
		}

		[[nodiscard]]
		const SvgSvgElement& GetElement() const
		{
			return _element;
		}

		explicit SvgDocument(const XmlElement& element);
		SvgDocument(SvgDocument&& other) noexcept = default;

	private:
		SvgDefs _defs;
		SvgSvgElement _element;

		friend class SvgElement;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGDOCUMENT_HPP
