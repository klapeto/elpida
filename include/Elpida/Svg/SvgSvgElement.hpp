//
// Created by klapeto on 22/12/2023.
//

#ifndef ELPIDA_SVG_SVGSVGELEMENT_HPP
#define ELPIDA_SVG_SVGSVGELEMENT_HPP

#include "SvgElement.hpp"
#include "SvgLength.hpp"
#include "SvgPreserveAspectRatio.hpp"
#include "SvgViewBox.hpp"
#include "SvgViewPort.hpp"

namespace Elpida
{
	class SvgSvgElement : public SvgElement
	{
	public:
		[[nodiscard]]
		const SvgViewBox& GetViewBox() const
		{
			return _viewBox;
		}

		[[nodiscard]]
		const SvgViewPort& GetViewPort() const
		{
			return _viewPort;
		}

		[[nodiscard]]
		const SvgPreserveAspectRatio& GetPreserveAspectRatio() const
		{
			return _preserveAspectRatio;
		}

		SvgSvgElement() = default;
		SvgSvgElement(const XmlElement& element, SvgDocument& document);
	private:
		SvgViewBox _viewBox;
		SvgViewPort _viewPort;
		SvgPreserveAspectRatio _preserveAspectRatio;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGSVGELEMENT_HPP
