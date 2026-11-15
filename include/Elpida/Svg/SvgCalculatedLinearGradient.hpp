/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 10/3/2024.
//

#ifndef ELPIDA_SVGCALCULATEDLINEARGRADIENT_HPP
#define ELPIDA_SVGCALCULATEDLINEARGRADIENT_HPP

#include <vector>
#include "Elpida/Svg/SvgLinearEquation.hpp"
#include "Elpida/Svg/SvgCalculatedGradientStop.hpp"
#include "Elpida/Svg/SvgCalculatedGradient.hpp"
#include "Elpida/Svg/SvgConfig.hpp"

namespace Elpida
{
	class SvgTransform;
	class SvgBounds;
	class SvgGradient;
	class SvgCalculationContext;

	class SvgCalculatedLinearGradient : public SvgCalculatedGradient
	{
	public:
		void Transform(const SvgTransform& transform);

		[[nodiscard]]
		SvgColor CalculateColor(const SvgPoint& point) const;

		SvgCalculatedLinearGradient(const SvgGradient& gradient,
				const SvgBounds& elementBounds,
				std::vector<SvgCalculatedGradientStop>&& stops,
				const SvgCalculationContext& calculationContext);
	private:
		SvgLinearEquation _equation;
		std::vector<SvgLinearEquation> _stopNormals;
		SvgTransform _transform;
		SvgFloat _length;

		[[nodiscard]]
		SvgColor CalculatePad(const SvgPoint& point) const;

		[[nodiscard]]
		SvgColor CalculateRepeat(const SvgPoint& point) const;

		[[nodiscard]]
		SvgColor CalculateReflect(const SvgPoint& point) const;

		[[nodiscard]]
		SvgColor CalculateColor(const SvgPoint& point,
				const SvgCalculatedGradientStop& stopA,
				const SvgLinearEquation& normalA,
				const SvgCalculatedGradientStop& stopB,
				const SvgLinearEquation& normalB) const;
	};

	// Inline (in the header) so that the per-sample color lookup inlines into SvgSuperSampler: an out-of-line call
	// returns SvgColor (16 bytes) in xmm0/xmm1 on x86_64 Linux but through memory on Windows, which made this
	// wrapper about twice as expensive there (docs/development/parity-checks.md). always_inline: clang's inlining
	// cost counts those memory copies, so without it the Windows build kept SvgCalculatedPaint's as a call.
	[[gnu::always_inline]] inline SvgColor SvgCalculatedLinearGradient::CalculateColor(const SvgPoint& point) const
	{
		// TODO: Eliminate branch, measure impact
		switch (_spreadType)
		{
		case SvgSpreadType::Pad:
			return CalculatePad(point);
		case SvgSpreadType::Reflect:
			return CalculateReflect(point);
		case SvgSpreadType::Repeat:
			return CalculateRepeat(point);
		default:
			return CalculatePad(point);
		}
	}

} // Elpida

#endif //ELPIDA_SVGCALCULATEDLINEARGRADIENT_HPP
