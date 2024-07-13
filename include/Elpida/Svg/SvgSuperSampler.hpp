//
// Created by klapeto on 8/3/2024.
//

#ifndef ELPIDA_SVGSUPERSAMPLER_HPP
#define ELPIDA_SVGSUPERSAMPLER_HPP

#include "Elpida/Svg/SvgColor.hpp"
#include "Elpida/Svg/SvgFillRule.hpp"
#include "Elpida/Svg/SvgPoint.hpp"
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
				double x,
				double y,
				const SvgCalculatedPaint& paint,
				SvgFillRule fillRule) const;

		[[nodiscard]]
		std::size_t GetSampleCount() const
		{
			return _subSamples;
		}

		SvgSuperSampler()
				:SvgSuperSampler(1)
		{
		}

		explicit SvgSuperSampler(std::size_t subSamples);
	private:
		std::size_t _subSamples;
	};
} // Elpida

#endif //ELPIDA_SVGSUPERSAMPLER_HPP
