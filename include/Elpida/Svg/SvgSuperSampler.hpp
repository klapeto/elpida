//
// Created by klapeto on 8/3/2024.
//

#ifndef ELPIDA_SVGSUPERSAMPLER_HPP
#define ELPIDA_SVGSUPERSAMPLER_HPP

#include "SvgColor.hpp"
#include "SvgFillRule.hpp"

namespace Elpida
{
	class SvgPolygon;
	class SvgCalculatedPaint;

	class SvgSuperSampler
	{
	public:
		[[nodiscard]]
		SvgColor CalculatePixelColor(const SvgPolygon& polygon,
				std::size_t x,
				std::size_t y,
				const SvgCalculatedPaint& paint,
				SvgFillRule fillRule) const;

		explicit SvgSuperSampler(std::size_t subSamples);
	private:
		std::size_t _subSamplesPerDimension;
		double _actualSubSamples;
		double _subSampleStep;
		static constexpr double sideHalf = 1.0 / 2.0;
	};
} // Elpida

#endif //ELPIDA_SVGSUPERSAMPLER_HPP
