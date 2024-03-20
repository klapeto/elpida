//
// Created by klapeto on 8/3/2024.
//

#include "Elpida/Svg/SvgSuperSampler.hpp"
#include "Elpida/Svg/SvgPolygon.hpp"
#include "Elpida/Svg/SvgCalculatedPaint.hpp"
#include "Elpida/Svg/SvgPoint.hpp"
#include <random>

namespace Elpida
{
	SvgSuperSampler::SvgSuperSampler(std::size_t subSamples)
	{
		_samplePoints.emplace_back();

		// we NEED predetermined randomness to produce the same result at every time
		std::mt19937 generator(12345);
		std::uniform_real_distribution<> distribution(-0.5, 0.5);

		for (std::size_t i = 0; i < subSamples - 1; ++i)
		{
			_samplePoints.emplace_back(distribution(generator), distribution(generator));
		}
	}

	SvgColor SvgSuperSampler::CalculatePixelColor(const SvgPolygon& polygon, double x, double y,
			const SvgCalculatedPaint& paint, SvgFillRule fillRule) const
	{
		double r = 0.0;
		double g = 0.0;
		double b = 0.0;
		double a = 0.0;

		// we take multiple samples inside the tiny area of the pixel.
		// We assume the pixel itself has a canvas, and we start at (0,0)
		// of the pixel and take samples and advance by a step at a time
		// (eg next sample will be (subSampleStep, 0), next (2 * subSampleStep, 0) etc).
		// in the end we average the total channels we have got.
		// TODO: Better pattern. eg on 1 Sample we take the top left sample which is not optimal
		for (auto& sampleOffset : _samplePoints)
		{
			SvgPoint point = SvgPoint(x, y) + sampleOffset;

			// TODO: Optimize with lambda. Measure impact of the removal of branch
			bool inside = fillRule == SvgFillRule::NonZero ? polygon.IsPointInsideNonZero(point)
														   : polygon.IsPointInsideEvenOdd(point);
			if (inside)
			{
				auto color = paint.CalculateColor(point);
				r += color.R();
				g += color.G();
				b += color.B();
				a += color.A();
			}
		}

		r /= _samplePoints.size();
		g /= _samplePoints.size();
		b /= _samplePoints.size();
		a /= _samplePoints.size();

		return { r, g, b, a };
	}
} // Elpida