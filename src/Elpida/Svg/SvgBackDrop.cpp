//
// Created by klapeto on 6/3/2024.
//

#include "Elpida/Svg/SvgBackDrop.hpp"
#include "Elpida/Svg/SvgBlender.hpp"
#include "Elpida/Svg/SvgCompositor.hpp"

#include <cmath>
#include <cstring>
#include <Elpida/Svg/SvgPoint.hpp>
#include <Elpida/Svg/SvgPolygon.hpp>
#include <Elpida/Svg/SvgRasterizerPaint.hpp>

namespace Elpida
{

	static double BlendColor(double Cs, double Cb, double ab, const SvgBlender& blender)
	{
		return (1.0 - ab) * Cs + ab * blender.Blend(Cb, Cs);
	}

	void SvgBackDrop::Draw(const SvgPolygon& polygon,
			const SvgRasterizerPaint& paint,
			SvgFillRule fillRule,
			SvgBlendMode blendMode,
			SvgCompositingMode compositingMode,
			const std::size_t subSamples)
	{
		const std::size_t subSamplesPerDimension = std::ceil(std::sqrt(subSamples));
		const double actualSubSamples = subSamplesPerDimension * subSamplesPerDimension;    // this is needed because we round the sqrt
		const double subSampleStep = 1.0 / subSamplesPerDimension;
		constexpr auto sideHalf = 1.0 / 2.0;

		SvgBlender blender(blendMode);
		SvgCompositor compositor(compositingMode);
		auto& bounds = polygon.GetBounds();
		std::size_t startY = std::floor(std::max(0.0, bounds.GetMinY()));
		std::size_t startX = std::floor(std::max(0.0, bounds.GetMinX()));
		std::size_t width = std::ceil(std::min((double)_width, bounds.GetWidth() + startX));
		std::size_t height = std::ceil(std::min((double)_height, bounds.GetHeight() + startY));

		for (std::size_t y = startY; y < height; ++y)
		{
			for (std::size_t x = startX; x < width; ++x)
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
				auto sampleX = x - sideHalf;
				auto sampleY = y - sideHalf;
				for (std::size_t i = 0; i < subSamplesPerDimension; ++i)
				{
					for (std::size_t j = 0; j < subSamplesPerDimension; ++j)
					{
						SvgPoint point(sampleX, sampleY);

						// TODO: Optimize with lambda. Measure impact of the removal of branch
						bool inside = fillRule == SvgFillRule::NonZero ? polygon.IsPointInsideNonZero(point) : polygon.IsPointInsideEvenOdd(point);
						if (inside)
						{
							auto color = paint.CalculateColor(point);
							r += color.R();
							g += color.G();
							b += color.B();
							a += color.A();
						}
						sampleX += subSampleStep;
					}
					sampleX = x - sideHalf;
					sampleY += subSampleStep;
				}
				
				r /= actualSubSamples;
				g /= actualSubSamples;
				b /= actualSubSamples;
				a /= actualSubSamples;

				// See https://www.w3.org/TR/2015/CR-compositing-1-20150113/#generalformula
				auto& backdropColor = _colorData[y * _width + x];
				auto as = a;
				auto ab = backdropColor.A();

				auto rCs = BlendColor(r, backdropColor.R(), ab, blender);
				auto gCs = BlendColor(g, backdropColor.G(), ab, blender);
				auto bCs = BlendColor(b, backdropColor.B(), ab, blender);

				rCs = compositor.Composite(as, rCs, ab, backdropColor.R());
				gCs = compositor.Composite(as, gCs, ab, backdropColor.G());
				bCs = compositor.Composite(as, bCs, ab, backdropColor.B());

				auto ao = as + ab * (1.0 - as);

				if (ao <= 0.0)
				{
					backdropColor = SvgColor(rCs, gCs, bCs, ao);
				}
				else
				{
					backdropColor = SvgColor(rCs / ao, gCs / ao, bCs / ao, ao);
				}
			}
		}
	}

	SvgBackDrop::SvgBackDrop(std::size_t width, std::size_t height)
			:_width(width), _height(height)
	{
		_colorData.resize(width * height);
	}

	SvgBackDrop::SvgBackDrop(SvgBackDrop&& other) noexcept
			:_colorData(std::move(other._colorData)), _width(other._width), _height(other._height)
	{
	}

	SvgBackDrop& SvgBackDrop::operator=(SvgBackDrop&& other) noexcept
	{
		_height = other._height;
		_width = other._width;
		_colorData = std::move(other._colorData);
		return *this;
	}
} // Elpida
