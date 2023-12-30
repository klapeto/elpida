//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGGRADIENTSTOP_HPP
#define ELPIDA_SVG_SVGGRADIENTSTOP_HPP

#include "SvgColor.hpp"
#include "SvgLength.hpp"
#include "SvgElement.hpp"

namespace Elpida
{
	class XmlElement;

	class SvgGradientStop : public SvgElement
	{
	public:
		[[nodiscard]]
		const SvgColor& GetColor() const
		{
			return _color;
		}

		[[nodiscard]]
		double GetOffset() const
		{
			return _offset;
		}

		[[nodiscard]]
		double GetOpacity() const
		{
			return _opacity;
		}

		explicit SvgGradientStop(const XmlElement& element, SvgDocument& document);
	private:
		SvgColor _color;
		double _offset;
		double _opacity;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGGRADIENTSTOP_HPP
