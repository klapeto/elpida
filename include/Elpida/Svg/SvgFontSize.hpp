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
		const SvgLength& GetLength() const
		{
			return _length;
		}

		SvgFontSize() = default;
		explicit SvgFontSize(std::string_view view);
	private:
		SvgLength _length;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGFONTSIZE_HPP
