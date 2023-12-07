//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGSHAPE_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGSHAPE_HPP

#include "SvgBounds.hpp"
#include "SvgPaint.hpp"
#include "SvgPath.hpp"
#include "SvgTransform.hpp"
#include <string>
#include <vector>

namespace Elpida
{

	enum class SvgLikeJoin
	{
		Miter = 0,
		Round = 1,
		Bevel = 2
	};

	enum class SvgLineCap
	{
		Butt = 0,
		Round = 1,
		Square = 2
	};

	enum class SvgFillRule
	{
		NonZero = 0,
		EvenOdd = 1
	};

	enum class SvgFlags
	{
		Visible = 1
	};

	class SvgShape
	{

	private:
		std::string _id;
		SvgPaint _fill;
		SvgPaint _stroke;
		float _opacity;
		float _strokeWidth;
		float _strokeDashOffset;
		float _strokeDashArray[8];
		char _strokeDashCount;
		SvgLikeJoin _strokeLikeJoin;
		SvgLineCap _strokeLineCap;
		float _miterLimit;
		SvgFillRule _fillRule;
		SvgFlags _flags;
		SvgBounds _bounds;
		std::string _fillGradientId;
		std::string _strokeGradientId;
		SvgTransform _transform;
		std::vector<SvgPath> _paths;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGSHAPE_HPP
