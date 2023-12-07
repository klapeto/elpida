//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENT_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENT_HPP

#include "SvgGradientStop.hpp"
#include "SvgTransform.hpp"
#include <vector>
namespace Elpida
{

	enum class SvgSpreadType
	{
		Pad = 0,
		Reflect = 1,
		Repeat = 2
	};

	class SvgGradient
	{

	private:
		std::vector<SvgGradientStop> _stops;
		SvgTransform _transform;
		SvgSpreadType _spreadType;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENT_HPP
