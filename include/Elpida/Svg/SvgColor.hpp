//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGCOLOR_HPP
#define ELPIDA_SVG_SVGCOLOR_HPP

#include <string_view>
#include <algorithm>
#include "SvgConfig.hpp"

namespace Elpida
{
	class SvgColor
	{
	public:

		static constexpr SvgFloat MaxValue = 1.0;
		static constexpr SvgFloat HalfValue = MaxValue / 2.0;
		static constexpr SvgFloat QuarterValue = HalfValue / 2.0;
		static constexpr SvgFloat MinValue = 0.0;

		static SvgColor Black()
		{
			return { SvgColor::MinValue, SvgColor::MinValue, SvgColor::MinValue, SvgColor::MaxValue };
		}

		[[nodiscard]]
		SvgFloat R() const
		{
			return _r;
		}

		[[nodiscard]]
		SvgFloat G() const
		{
			return _g;
		}

		[[nodiscard]]
		SvgFloat B() const
		{
			return _b;
		}

		[[nodiscard]]
		SvgFloat A() const
		{
			return _a;
		}

		[[nodiscard]]
		SvgColor WithMultipliedAplha(const SvgFloat alpha) const
		{
			SvgFloat a = _a * alpha;
			a = std::max(MinValue, std::min(MaxValue, a));

			return SvgColor(_r, _g, _b, a);
		}

		SvgColor()
				:_r(MinValue), _g(MinValue), _b(MinValue), _a(MinValue)
		{

		}

		SvgColor(const SvgFloat r, const SvgFloat g, const SvgFloat b, const SvgFloat a)
				:_r(r),
				 _g(g),
				 _b(b),
				 _a(a)
		{
		}

		explicit SvgColor(std::string_view view);

	private:
		SvgFloat _r;
		SvgFloat _g;
		SvgFloat _b;
		SvgFloat _a;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGCOLOR_HPP
