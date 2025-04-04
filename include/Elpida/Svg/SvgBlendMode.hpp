//
// Created by klapeto on 7/3/2024.
//

#ifndef ELPIDA_SVG_SVGBLENDMODE_HPP
#define ELPIDA_SVG_SVGBLENDMODE_HPP

#include <string_view>

namespace Elpida
{
	enum class SvgBlendMode
	{
		Normal,
		Multiply,
		Screen,
		Overlay,
		Darken,
		Lighten,
		ColorDodge,
		ColorBurn,
		HardLight,
		SoftLight,
		Difference,
		Exclusion
	};

	class SvgBlendModeParser
	{
	public:
		static SvgBlendMode Parse(std::string_view view);
	};
} //Elpida

#endif //ELPIDA_SVG_SVGBLENDMODE_HPP
