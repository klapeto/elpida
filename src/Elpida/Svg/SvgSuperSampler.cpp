//
// Created by klapeto on 8/3/2024.
//

#include <cmath>
#include "Elpida/Svg/SvgSuperSampler.hpp"
#include "Elpida/Svg/SvgPolygon.hpp"
#include "Elpida/Svg/SvgRasterizerPaint.hpp"
#include "Elpida/Svg/SvgPoint.hpp"

namespace Elpida
{
	SvgSuperSampler::SvgSuperSampler(std::size_t subSamples)
	{
		_subSamplesPerDimension = std::ceil(std::sqrt(subSamples));
		_actualSubSamples =
				_subSamplesPerDimension * _subSamplesPerDimension;    // this is needed because we round the sqrt
		_subSampleStep = 1.0 / _subSamplesPerDimension;
	}

	SvgColor SvgSuperSampler::CalculatePixelColor(const SvgPolygon& polygon, std::size_t x, std::size_t y,
			const SvgRasterizerPaint& paint, SvgFillRule fillRule) const
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
		auto sampleX = x - sideHalf;
		auto sampleY = y - sideHalf;
		for (std::size_t i = 0; i < _subSamplesPerDimension; ++i)
		{
			for (std::size_t j = 0; j < _subSamplesPerDimension; ++j)
			{
				SvgPoint point(sampleX, sampleY);

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
				sampleX += _subSampleStep;
			}
			sampleX = x - sideHalf;
			sampleY += _subSampleStep;
		}

		r /= _actualSubSamples;
		g /= _actualSubSamples;
		b /= _actualSubSamples;
		a /= _actualSubSamples;

		return { r, g, b, a };
	}
} // Elpida