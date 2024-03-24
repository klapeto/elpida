//
// Created by klapeto on 7/3/2024.
//

#ifndef ELPIDA_SVGCOMPOSITOR_HPP
#define ELPIDA_SVGCOMPOSITOR_HPP

#include "SvgColor.hpp"
#include "SvgCompositingMode.hpp"

namespace Elpida
{
	class SvgCompositor
	{
	public:
		[[nodiscard]]
		double Composite(const double as, const double Cs, const double ab, const double Cb) const
		{
			return _compositor(as, Cs, ab, Cb);
		}

		[[nodiscard]]
		SvgColor Composite(const SvgColor &Cs, const SvgColor &Cb) const
		{
			return {
				Composite(Cs.A(), Cs.R(), Cb.A(), Cb.R()),
				Composite(Cs.A(), Cs.G(), Cb.A(), Cb.G()),
				Composite(Cs.A(), Cs.B(), Cb.A(), Cb.B()),
				Composite(Cs.A(), 1.0, Cb.A(), 1.0)
			};
		}

		explicit SvgCompositor(SvgCompositingMode mode);

	private:
		using Compositor = double (*)(double, double, double, double);
		Compositor _compositor;
	};
} // Elpida

#endif //ELPIDA_SVGCOMPOSITOR_HPP
