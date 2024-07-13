//
// Created by klapeto on 10/3/2024.
//

#ifndef ELPIDA_SVGCALCULATEDGRADIENT_HPP
#define ELPIDA_SVGCALCULATEDGRADIENT_HPP

#include <vector>

#include "Elpida/Svg/SvgCalculatedGradientStop.hpp"
#include "Elpida/Svg/SvgColor.hpp"
#include "Elpida/Svg/SvgSpreadType.hpp"

namespace Elpida
{

	class SvgCalculatedGradientStop;

	class SvgCalculatedGradient
	{
	public:
		SvgCalculatedGradient(std::vector<SvgCalculatedGradientStop>&& stops, SvgSpreadType spreadType);
	protected:
		std::vector<SvgCalculatedGradientStop> _stops;
		SvgSpreadType _spreadType;

		static SvgColor InterpolateColor(const SvgCalculatedGradientStop& stopA,
				const SvgCalculatedGradientStop& stopB,
				double ratio);
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDGRADIENT_HPP
