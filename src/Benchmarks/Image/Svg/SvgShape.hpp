//
// Created by klapeto on 3/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGSHAPE_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGSHAPE_HPP

#include "SvgFillRule.hpp"
#include "SvgFlags.hpp"
#include "SvgLineCap.hpp"
#include "SvgLineJoin.hpp"
#include "SvgPaint.hpp"
#include "SvgPath.hpp"
#include "SvgTransform.hpp"
#include <string>
#include <vector>
namespace Elpida
{

	class SvgShape
	{

	private:
		std::vector<SvgPath> _paths;
		std::string _id;
		SvgPaint _fill;
		SvgPaint _stroke;
		float _opacity;
		float _strokeWidth;
		float _strokeDashOffset;
		float _strokeDashArray[8];
		int _strokeDashCount;
		SvgLineJoin _strokeLikeJoin;
		SvgLineCap _strokeLikeCap;
		float _miterLimit;
		SvgFillRule _fillRule;
		SvgFlags _flags;
		float bounds[4];
		std::string _fillGradient;
		std::string _strokeGradient;
		SvgTransform _transform;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGSHAPE_HPP
