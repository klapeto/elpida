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

	class SvgDocument final
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

		SvgDocument() = default;
		explicit SvgDocument(const XmlElement& element);
		SvgDocument(const SvgDocument&) = default;
		SvgDocument& operator=(const SvgDocument& other) = default;
		SvgDocument(SvgDocument&& other) noexcept = default;
		SvgDocument& operator=(SvgDocument&& other) noexcept = default;
		~SvgDocument() = default;
	private:
		SvgDefs _defs;
		SvgSvgElement _element;

		friend class SvgElement;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGDOCUMENT_HPP
