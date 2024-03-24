//
// Created by klapeto on 7/3/2024.
//

#ifndef ELPIDA_SVGBLENDER_HPP
#define ELPIDA_SVGBLENDER_HPP

#include "Elpida/Svg/SvgBlendMode.hpp"
#include "Elpida/Svg/SvgColor.hpp"

namespace Elpida
{

	class SvgBlender final
	{
	public:
		[[nodiscard]]
		double Blend(const double Cb, const double Cs) const
		{
			return _blender(Cb, Cs);
		}

		[[nodiscard]]
		double Blend(const double Cs, const double Cb, const double ab) const
		{
			return (1.0 - ab) * Cs + ab * Blend(Cb, Cs);
		}

		[[nodiscard]]
		SvgColor Blend(const SvgColor& Cs, const SvgColor& Cb) const
		{
			return {
				Blend(Cs.R(), Cb.R(), Cb.A()),
				Blend(Cs.G(), Cb.G(), Cb.A()),
				Blend(Cs.B(), Cb.B(), Cb.A()),
				Cs.A()
			};
		}

		explicit SvgBlender(SvgBlendMode mode);

	private:
		using Blender = double (*)(double, double);
		Blender _blender;
	};

} // Elpida

#endif //ELPIDA_SVGBLENDER_HPP
