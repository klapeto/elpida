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
// Created by klapeto on 9/3/2024.
//

#ifndef ELPIDA_SVGCALCULATEDSTROKE_HPP
#define ELPIDA_SVGCALCULATEDSTROKE_HPP

#include <vector>
#include "Elpida/Svg/SvgLineCap.hpp"
#include "Elpida/Svg/SvgLineJoin.hpp"
#include "Elpida/Svg/SvgCalculatedPaint.hpp"
#include "Elpida/Svg/SvgConfig.hpp"

namespace Elpida
{

	class SvgStroke;
	class SvgCalculationContext;

	class SvgCalculatedStroke : public SvgCalculatedPaint
	{
	public:
		[[nodiscard]]
		const std::vector<SvgFloat>& GetDashes() const
		{
			return _dashes;
		}

		[[nodiscard]]
		SvgLineJoin GetLineJoin() const
		{
			return _lineJoin;
		}

		[[nodiscard]]
		SvgLineCap GetLineCap() const
		{
			return _lineCap;
		}

		[[nodiscard]]
		SvgFloat GetDashOffset() const
		{
			return _dashOffset;
		}

		[[nodiscard]]
		SvgFloat GetWidth() const
		{
			return _width;
		}

		[[nodiscard]]
		SvgFloat GetMiterLimit() const
		{
			return _miterLimit;
		}

		void Transform(const SvgTransform& transform) override;

		SvgCalculatedStroke(const SvgStroke& stroke, const SvgBounds& elementBounds, const SvgDocument& document,
				const SvgCalculationContext& calculatingContext);
		SvgCalculatedStroke(const SvgCalculatedStroke&) = default;
		SvgCalculatedStroke& operator=(const SvgCalculatedStroke&) = default;
		SvgCalculatedStroke(SvgCalculatedStroke&&) noexcept = default;
		SvgCalculatedStroke& operator=(SvgCalculatedStroke&&) noexcept = default;
		~SvgCalculatedStroke() override = default;
	private:
		std::vector<SvgFloat> _dashes;
		SvgLineJoin _lineJoin;
		SvgLineCap _lineCap;
		SvgFloat _dashOffset;
		SvgFloat _width;
		SvgFloat _miterLimit;
	};

} // Svg

#endif //ELPIDA_SVGCALCULATEDSTROKE_HPP
