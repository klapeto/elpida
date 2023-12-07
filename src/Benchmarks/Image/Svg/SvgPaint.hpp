//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGPAINT_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGPAINT_HPP

#include "SvgColor.hpp"
#include "SvgGradient.hpp"
#include <memory>
namespace Elpida
{

	enum class SvgPaintType
	{
		Undefined = -1,
		None = 0,
		Color = 1,
		LinearGradient = 2,
		RadialGradient = 3
	};

	class SvgPaint
	{

	private:
		SvgPaintType _type;
		union
		{
			SvgColor _color;
			std::shared_ptr<SvgGradient> _gradient;
		};
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGPAINT_HPP
