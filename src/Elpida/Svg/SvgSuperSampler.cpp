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
		if (subSamples == 0)
		{
			subSamples = 1;
		}

		auto generator = std::mt19937(12345); // NOLINT(*-msc51-cpp)
		auto distribution = std::uniform_real_distribution<SvgFloat>(-0.5, 0.5);
		_subSamplesOffsets.emplace_back(0, 0);
		for (std::size_t i = 0; i < subSamples - 1; ++i)
		{
			_subSamplesOffsets.emplace_back(distribution(generator), distribution(generator));
		}

		// we NEED predetermined randomness to produce the same result at every time
	}

	static void CalculateSample(const SvgPolygon& polygon, SvgFloat x, SvgFloat y, const SvgCalculatedPaint& paint,
			SvgFillRule fillRule, const SvgPoint& sampleOffset, SvgFloat& r, SvgFloat& g, SvgFloat& b, SvgFloat& a)
	{
		SvgPoint point = SvgPoint(x + SvgFloat(0.5), y + SvgFloat(0.5)) + sampleOffset;

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

	SvgColor SvgSuperSampler::CalculatePixelColor(const SvgPolygon& polygon, SvgFloat x, SvgFloat y,
			const SvgCalculatedPaint& paint, SvgFillRule fillRule) const
	{
		SvgFloat r = 0.0;
		SvgFloat g = 0.0;
		SvgFloat b = 0.0;
		SvgFloat a = 0.0;

		const SvgFloat subSamples = _subSamplesOffsets.size();

		// we take multiple samples inside the tiny area of the pixel.
		// We assume the pixel itself has a canvas, and we start at (0,0)
		// of the pixel and take samples and advance by a step at a time
		// (eg next sample will be (subSampleStep, 0), next (2 * subSampleStep, 0) etc).
		// in the end we average the total channels we have got.

		for (auto& sampleOffset : _subSamplesOffsets)
		{
			CalculateSample(polygon, x, y, paint, fillRule, sampleOffset, r, g, b, a);
		}

		r /= subSamples;
		g /= subSamples;
		b /= subSamples;
		a /= subSamples;

		return { r, g, b, a * paint.Opacity() };
	}
} // Elpida