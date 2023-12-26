//
// Created by klapeto on 24/12/2023.
//

#ifndef ELPIDA_SVG_SVGFONTSIZE_HPP
#define ELPIDA_SVG_SVGFONTSIZE_HPP

#include <string_view>

#include "SvgLength.hpp"

namespace Elpida
{
	class SvgFontSize
	{
	public:

		[[nodiscard]]
		double CalculateActualValue(const double parentFontSize) const
		{
			return _length.CalculateActualValue(0.0, parentFontSize, parentFontSize, 0.0);
		}

		SvgFontSize() = default;
		explicit SvgFontSize(std::string_view view);
	private:
		SvgLength _length;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGFONTSIZE_HPP
