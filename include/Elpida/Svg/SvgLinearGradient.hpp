//
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_SVG_SVGLINEARGRADIENT_HPP
#define ELPIDA_SVG_SVGLINEARGRADIENT_HPP

#include "SvgLength.hpp"
#include "SvgGradient.hpp"

namespace Elpida
{
	class SvgLinearGradient: public SvgGradient
	{
	public:
		[[nodiscard]] const SvgLength& GetX1() const
		{
			return _x1;
		}

		[[nodiscard]] const SvgLength& GetY1() const
		{
			return _y1;
		}

		[[nodiscard]] const SvgLength& GetX2() const
		{
			return _x2;
		}

		[[nodiscard]] const SvgLength& GetY2() const
		{
			return _y2;
		}

		explicit SvgLinearGradient(const XmlElement& element, SvgDocument& document);
	private:
		SvgLength _x1;
		SvgLength _y1;
		SvgLength _x2;
		SvgLength _y2;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGLINEARGRADIENT_HPP
