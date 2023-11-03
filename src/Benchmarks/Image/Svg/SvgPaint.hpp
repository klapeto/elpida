//
// Created by klapeto on 3/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGPAINT_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGPAINT_HPP

#include "SvgPaintType.hpp"
#include "SvgColor.hpp"
namespace Elpida
{

	class SvgGradient;

	class SvgPaint
	{
	private:
		SvgPaintType _type;
		union {
			SvgColor _color;
			SvgGradient* _gradient;
		};
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGPAINT_HPP
