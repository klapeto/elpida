//
// Created by klapeto on 3/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGATTRIBUTE_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGATTRIBUTE_HPP

#include "SvgColor.hpp"
#include "SvgFillRule.hpp"
#include "SvgLineCap.hpp"
#include "SvgLineJoin.hpp"
#include "SvgTransform.hpp"
#include <string>
#include <vector>

namespace Elpida
{
	class SvgAttribute
	{
	public:
		SvgAttribute(const std::string_view& name, const std::string_view& value);
	private:
		std::string _id;
		SvgTransform _transform;
		SvgColor _fillColor;
		SvgColor _strokeColor;
		float _opacity;
		float _fillOpacity;
		float _strokeOpacity;
		std::string _fillGradient;
		std::string _strokeGradient;
		float _strokeWidth;
		float _strokeDashOffset;
		std::vector<float> _strokeDashArray;
		SvgLineJoin _strokeLineJoin;
		SvgLineCap _strokeLineCap;
		float _miterLimit;
		SvgFillRule _fillRule;
		float _fontSize;
		SvgColor _stopColor;
		float _stopOpacity;
		float _stopOffset;
		bool _hasFill;
		bool _hasStroke;
		bool _visible;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGATTRIBUTE_HPP
