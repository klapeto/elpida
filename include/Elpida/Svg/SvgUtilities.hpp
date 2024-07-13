//
// Created by klapeto on 15/4/2024.
//

#ifndef ELPIDA_SVGUTILITIES_HPP
#define ELPIDA_SVGUTILITIES_HPP

namespace Elpida
{
	class SvgUtilities
	{
	public:
		static inline constexpr double Pi = 3.141592653589793238462643383279502884;

		template<typename T>
		static constexpr T Lerp(T a, T b, T t) noexcept
		{
			if ((a <= 0 && b >= 0) || (a >= 0 && b <= 0))
			{
				return t * b + (1 - t) * a;
			}

			if (t == 1)
			{
				return b;
			}                        // exact

			// Exact at t=0, monotonic except near t=1,
			// bounded, determinate, and consistent:
			const T x = a + t * (b - a);
			return (t > 1) == (b > a)
				   ? (b < x ? x : b)
				   : (b > x ? x : b);  // monotonic near t=1
		}

		static constexpr double Lerp(double a, double b, double t) noexcept
		{
			return Lerp<double>(a, b, t);
		}

		SvgUtilities() = delete;
	};
}

#endif //ELPIDA_SVGUTILITIES_HPP
