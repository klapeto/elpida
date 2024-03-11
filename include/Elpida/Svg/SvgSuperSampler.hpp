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

		explicit SvgSuperSampler(std::size_t subSamples);
	private:
		std::vector<SvgPoint> _samplePoints;
	};
} // Elpida

#endif //ELPIDA_SVGSUPERSAMPLER_HPP
