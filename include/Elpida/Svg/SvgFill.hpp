//
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_SVG_SVGFILL_HPP
#define ELPIDA_SVG_SVGFILL_HPP

#include "SvgPaint.hpp"
#include "SvgFillRule.hpp"

namespace Elpida
{
	class SvgFill : public SvgPaint
	{
	public:
		[[nodiscard]]
		SvgFillRule GetFillRule() const
		{
			return _fillRule;
		}

		explicit SvgFill(const XmlMap& properties);
		SvgFill()
			: SvgPaint(), _fillRule(SvgFillRule::NonZero)
		{
		}
	private:
		SvgFillRule _fillRule;
		void ParseFillRule(const std::string& value);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGFILL_HPP
