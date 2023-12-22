//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGGRADIENTSTOP_HPP
#define ELPIDA_SVG_SVGGRADIENTSTOP_HPP

#include "SvgColor.hpp"
#include "SvgCoordinate.hpp"
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
		const SvgCoordinate& GetOffset() const
		{
			return _offset;
		}

		[[nodiscard]]
		const SvgCoordinate& GetOpacity() const
		{
			return _opacity;
		}

		explicit SvgGradientStop(const XmlElement& element, SvgDefs& defs);
	private:
		SvgColor _color;
		SvgCoordinate _offset;
		SvgCoordinate _opacity;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGGRADIENTSTOP_HPP
