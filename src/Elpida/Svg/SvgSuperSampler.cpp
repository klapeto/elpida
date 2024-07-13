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
			:_subSamples(subSamples)
	{
		if (_subSamples == 0)
		{
			_subSamples = 1;
		}
		// we NEED predetermined randomness to produce the same result at every time
	}

	static void CalculateSample(const SvgPolygon& polygon, double x, double y, const SvgCalculatedPaint& paint,
			SvgFillRule fillRule, const SvgPoint& sampleOffset, double& r, double& g, double& b, double& a)
	{
		SvgPoint point = SvgPoint(x + 0.5, y + 0.5) + sampleOffset;

		// TODO: Optimize with lambda. Measure impact of the removal of branch
		bool inside = fillRule == SvgFillRule::NonZero ? polygon.IsPointInsideNonZero(point)
													   : polygon.IsPointInsideEvenOdd(point);

		auto color = paint.CalculateColor(point);
		r += color.R();
		g += color.G();
		b += color.B();

		if (inside)
		{
			a += color.A();
		}
	}

	SvgColor SvgSuperSampler::CalculatePixelColor(const SvgPolygon& polygon, double x, double y,
			const SvgCalculatedPaint& paint, SvgFillRule fillRule) const
	{
		double r = 0.0;
		double g = 0.0;
		double b = 0.0;
		double a = 0.0;

		const double subSamples = _subSamples;

		// we take multiple samples inside the tiny area of the pixel.
		// We assume the pixel itself has a canvas, and we start at (0,0)
		// of the pixel and take samples and advance by a step at a time
		// (eg next sample will be (subSampleStep, 0), next (2 * subSampleStep, 0) etc).
		// in the end we average the total channels we have got.

		CalculateSample(polygon, x, y, paint, fillRule, SvgPoint(0, 0), r, g, b, a);

		if (_subSamples > 1)
		{
			auto generator = std::mt19937(12345); // NOLINT(*-msc51-cpp)
			auto distribution = std::uniform_real_distribution<>(-0.5, 0.5);
			for (std::size_t i = 0; i < _subSamples - 1; ++i)
			{
				SvgPoint sampleOffset(distribution(generator), distribution(generator));
				CalculateSample(polygon, x, y, paint, fillRule, sampleOffset, r, g, b, a);
			}
		}


		r /= subSamples;
		g /= subSamples;
		b /= subSamples;
		a /= subSamples;

		return { r, g, b, a * paint.Opacity() };
	}
} // Elpida