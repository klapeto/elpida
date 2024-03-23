//
// Created by klapeto on 7/3/2024.
//

#ifndef ELPIDA_SVGCOMPOSITOR_HPP
#define ELPIDA_SVGCOMPOSITOR_HPP

#include "SvgCompositingMode.hpp"

namespace Elpida
{

	class SvgCompositor
	{
	public:

		[[nodiscard]]
		double Composite(double as, double Cs, double ab, double Cb) const
		{
			return _compositor(as, Cs, ab, Cb);
		}

		explicit SvgCompositor(SvgCompositingMode mode);
	private:
		using Compositor = double (*)(double, double, double, double);
		Compositor _compositor;
	};

} // Elpida

#endif //ELPIDA_SVGCOMPOSITOR_HPP
