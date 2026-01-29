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
// Created by klapeto on 8/3/2024.
//

#ifndef ELPIDA_SVGSUPERSAMPLER_HPP
#define ELPIDA_SVGSUPERSAMPLER_HPP

#include "Elpida/Svg/SvgColor.hpp"
#include "Elpida/Svg/SvgFillRule.hpp"
#include "Elpida/Svg/SvgPoint.hpp"
#include "Elpida/Svg/SvgConfig.hpp"
#include <vector>

namespace Elpida
{
	class SvgPolygon;
	class SvgCalculatedPaint;

	class SvgSuperSampler
	{
	public:
		[[nodiscard]]
		SvgColor CalculatePixelColor(const SvgPolygon& polygon,
				SvgFloat x,
				SvgFloat y,
				const SvgCalculatedPaint& paint,
				SvgFillRule fillRule) const;

		[[nodiscard]]
		std::size_t GetSampleCount() const
		{
			return _subSamplesOffsets.size();
		}

		SvgSuperSampler()
				:SvgSuperSampler(1)
		{
		}

		explicit SvgSuperSampler(std::size_t subSamples);
	private:
		std::vector<SvgPoint> _subSamplesOffsets;
	};
} // Elpida

#endif //ELPIDA_SVGSUPERSAMPLER_HPP
