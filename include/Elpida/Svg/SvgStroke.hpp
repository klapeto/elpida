//
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_SVG_SVGSTROKE_HPP
#define ELPIDA_SVG_SVGSTROKE_HPP

#include <vector>

#include "SvgLineJoin.hpp"
#include "SvgLineCap.hpp"
#include "SvgPaint.hpp"
#include "SvgLength.hpp"

namespace Elpida
{
	class SvgDocument;

	class SvgStroke : public SvgPaint
	{
	public:
		[[nodiscard]]
		const std::vector<SvgLength>& GetDashes() const
		{
			return _dashes;
		}

		[[nodiscard]]
		SvgLineJoin GetLineJoin() const
		{
			return _lineJoin;
		}

		[[nodiscard]]
		SvgLineCap GetLineCap() const
		{
			return _lineCap;
		}

		[[nodiscard]]
		const SvgLength& GetDashOffset() const
		{
			return _dashOffset;
		}

		[[nodiscard]]
		const SvgLength& GetWidth() const
		{
			return _width;
		}

		[[nodiscard]]
		SvgFloat GetMiterLimit() const
		{
			return _miterLimit;
		}

		SvgStroke()
			: SvgPaint(), _lineJoin(SvgLineJoin::Miter), _lineCap(SvgLineCap::Butt), _miterLimit(4.0), _width(1.0, SvgUnits::Px)
		{
		}
		explicit SvgStroke(const XmlMap& properties);
	private:
		std::vector<SvgLength> _dashes;
		SvgLineJoin _lineJoin;
		SvgLineCap _lineCap;
		SvgLength _dashOffset;
		SvgLength _width;
		SvgFloat _miterLimit;

		void ParseDashArray(const std::string& value);
		void ParseWidth(const std::string& value);
		void ParseLineCap(const std::string& value);
		void ParseLineJoin(const std::string& value);
		void ParseMiterLimit(const std::string& value);
		void ParseDashOffset(const std::string& dashOffset);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGSTROKE_HPP
