//
// Created by klapeto on 7/3/2024.
//

#ifndef ELPIDA_SVGBLENDER_HPP
#define ELPIDA_SVGBLENDER_HPP

#include "SvgBlendMode.hpp"

namespace Elpida
{

	class SvgBlender final
	{
	public:
		[[nodiscard]]
		double Blend(double Cb, double Cs) const
		{
			return _blender(Cb, Cs);
		}

		[[nodiscard]]
		double Blend(double Cs, double Cb, double ab) const
		{
			return (1.0 - ab) * Cs + ab * Blend(Cb, Cs);
		}

		explicit SvgBlender(SvgBlendMode mode);

	private:
		using Blender = double (*)(double, double);
		Blender _blender;
	};

} // Elpida

#endif //ELPIDA_SVGBLENDER_HPP
