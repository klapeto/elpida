//
// Created by klapeto on 10/3/2024.
//

#include "Elpida/Svg/SvgCalculatedGradient.hpp"

namespace Elpida
{
	SvgColor SvgCalculatedGradient::InterpolateColor(const SvgCalculatedGradientStop& stopA,
			const SvgCalculatedGradientStop& stopB, SvgFloat ratio)
	{
		auto tR = stopA.GetColor().R() * ratio;
		auto tG = stopA.GetColor().G() * ratio;
		auto tB = stopA.GetColor().B() * ratio;
		auto tA = (stopA.GetColor().A() * stopA.GetOpacity()) * ratio;

		tR += stopB.GetColor().R() * (SvgFloat(1.0) - ratio);
		tG += stopB.GetColor().G() * (SvgFloat(1.0) - ratio);
		tB += stopB.GetColor().B() * (SvgFloat(1.0) - ratio);
		tA += (stopB.GetColor().A() * stopB.GetOpacity()) * (SvgFloat(1.0) - ratio);

		return { tR, tG, tB, tA };
	}

	SvgCalculatedGradient::SvgCalculatedGradient(std::vector<SvgCalculatedGradientStop>&& stops,
			SvgSpreadType spreadType)
			:_stops(std::move(stops)), _spreadType(spreadType)
	{

	}
} // Elpida