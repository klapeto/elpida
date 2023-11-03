//
// Created by klapeto on 3/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENT_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENT_HPP

#include "SvgGradientStop.hpp"
#include "SvgSpreadType.hpp"
#include <vector>
namespace Elpida
{

	class SvgGradient
	{
	private:
		std::vector<SvgGradientStop> _stops;
		float _xform[6];
		SvgSpreadType _spreadType;
		float _fx;
		float _fy;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENT_HPP
