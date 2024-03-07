//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGCOLOR_HPP
#define ELPIDA_SVG_SVGCOLOR_HPP

#include <string_view>

namespace Elpida
{
	class SvgColor
	{
	public:

		static constexpr double MaxValue = 1.0;
		static constexpr double HalfValue = MaxValue / 2.0;
		static constexpr double QuarterValue = HalfValue / 2.0;
		static constexpr double MinValue = 0.0;

		[[nodiscard]]
		double R() const
		{
			return _r;
		}

		[[nodiscard]]
		double G() const
		{
			return _g;
		}

		[[nodiscard]]
		double B() const
		{
			return _b;
		}

		[[nodiscard]]
		double A() const
		{
			return _a;
		}

		[[nodiscard]]
		SvgColor WithMultipliedAplha(const double alpha) const
		{
			double a = _a * alpha;
			a = std::max(MinValue, std::min(MaxValue, a));

			return SvgColor(_r, _g, _b, a);
		}

		SvgColor()
			: _r(MinValue), _g(MinValue), _b(MinValue), _a(MinValue)
		{

		}

		SvgColor(const double r, const double g, const double b, const double a)
			: _r(r),
			  _g(g),
			  _b(b),
			  _a(a)
		{
		}

		explicit SvgColor(std::string_view view);

	private:
		double _r;
		double _g;
		double _b;
		double _a;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGCOLOR_HPP
