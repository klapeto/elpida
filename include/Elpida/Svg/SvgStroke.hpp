//
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_SVG_SVGSTROKE_HPP
#define ELPIDA_SVG_SVGSTROKE_HPP

#include <vector>

#include "SvgLineJoin.hpp"
#include "SvgLineCap.hpp"
#include "SvgPaint.hpp"

namespace Elpida
{
	class SvgDocument;

	class SvgStroke : public SvgPaint
	{
	public:
		void ParseDashOffset(const SvgDocument& document, const std::string& dashOffset);
		SvgStroke(const XmlMap& properties, const SvgDocument& document);
		SvgStroke()
			: SvgPaint(), _lineJoin(SvgLineJoin::Miter), _lineCap(SvgLineCap::Butt), _dashOffset(0), _width(1.0), _miterLimit(0.0)
		{
		}
	private:
		std::vector<double> _dashes;
		SvgLineJoin _lineJoin;
		SvgLineCap _lineCap;
		double _dashOffset;
		double _width;
		double _miterLimit;

		void ParseDashArray(const std::string& value, const SvgDocument& document);
		void ParseWidth(const std::string& value, const SvgDocument& document);
		void ParseLineCap(const std::string& value);
		void ParseLineJoin(const std::string& value);
		void ParseMiterLimit(const std::string& value);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGSTROKE_HPP
