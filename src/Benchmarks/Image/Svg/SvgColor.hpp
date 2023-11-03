//
// Created by klapeto on 3/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGCOLOR_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGCOLOR_HPP

namespace Elpida
{
	using SvgColor = unsigned int;

	constexpr SvgColor FromRGB(unsigned char r, unsigned char g, unsigned char b)
	{
		return ((unsigned int)r) | ((unsigned int)g << 8) | ((unsigned int)b << 16);
	}
}

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGCOLOR_HPP
