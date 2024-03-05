//
// Created by klapeto on 24/12/2023.
//

#include "Elpida/Svg/SvgRasterizer.hpp"

#include <algorithm>
#include <iostream>
#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgGradient.hpp>
#include <Elpida/Svg/SvgPaint.hpp>
#include <Elpida/Svg/SvgEdge.hpp>

#include <cstring>
#include <list>
#include <memory>
#include <Elpida/Svg/SvgRasterizerPaint.hpp>
#include <Elpida/Svg/SvgShapePolygonizer.hpp>

namespace Elpida
{
	void SvgRasterizer::Rasterize(const SvgDocument& document,
		unsigned char* outputBuffer,
		std::size_t width,
		std::size_t height,
		std::size_t stride,
		double tx,
		double ty,
		double scale,
		std::size_t subSamples)
	{
		std::memset(outputBuffer, 0, width * 4);

		const std::size_t subSamplesPerDimension = std::ceil(sqrt(subSamples));
		const auto actualSubSamples = subSamplesPerDimension * subSamplesPerDimension;	// this is needed because we round the sqrt
		const double subSampleStep = 1.0 / subSamplesPerDimension;
		constexpr auto sideHalf = 1.0 / 2.0;
		for (auto& element : document.GetElement().GetChildren())
		{
			if (!element.IsVisible()) continue;
			if (element.GetFill().IsSet())
			{
				auto polygon = SvgShapePolygonizer::Polygonize(element.GetPaths(), scale);

				auto paint = SvgRasterizerPaint(element.GetFill(), element, document);
				for (std::size_t y = 0; y < height; ++y)
				{
					for (std::size_t x = 0; x < width; ++x)
					{
						double r = 0.0;
						double g = 0.0;
						double b = 0.0;
						double a = 0.0;

						auto sampleX = x - sideHalf;
						auto sampleY = y - sideHalf;
						for (std::size_t i = 0; i < subSamplesPerDimension; ++i)
						{
							for (std::size_t j = 0; j < subSamplesPerDimension; ++j)
							{
								SvgPoint point(sampleX, sampleY);
								if (polygon.IsPointInsideEvenOdd(point))
								{
									auto color = paint.CalculateColor(point, document);
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

						outputBuffer[(y * width * 4 + x * 4)] = r / actualSubSamples;
						outputBuffer[(y * width * 4 + x * 4) + 1] = g / actualSubSamples;
						outputBuffer[(y * width * 4 + x * 4) + 2] = b / actualSubSamples;
						outputBuffer[(y * width * 4 + x * 4) + 3] = a / actualSubSamples;
					}
				}
			}

			if (element.GetStroke().IsSet())
			{
				std::vector<SvgEdge> edges;
			}
		}

		//UnpremultiplyAlpha(outputBuffer, width, height, stride);
	}
} // Elpida
