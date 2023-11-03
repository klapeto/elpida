//
// Created by klapeto on 3/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENTSTOP_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENTSTOP_HPP

#include "SvgColor.hpp"
namespace Elpida
{

	class SvgGradientStop
	{
	private:
		SvgColor _color;
		float _offset;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENTSTOP_HPP
