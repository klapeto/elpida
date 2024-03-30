//
// Created by klapeto on 6/3/2024.
//

#include "Elpida/Svg/SvgBackDrop.hpp"
#include "Elpida/Svg/SvgBlender.hpp"
#include "Elpida/Svg/SvgCompositor.hpp"
#include "Elpida/Svg/SvgSuperSampler.hpp"

#include <cmath>
#include <Elpida/Svg/SvgPolygon.hpp>
#include <Elpida/Svg/SvgCalculatedPaint.hpp>
#include <thread>

namespace Elpida
{
	void SvgBackDrop::Draw(const SvgPolygon& polygon,
			const SvgCalculatedPaint& paint,
			SvgFillRule fillRule,
			SvgBlendMode blendMode,
			SvgCompositingMode compositingMode,
			const std::size_t subSamples)
	{
		const SvgBlender blender(blendMode);
		const SvgCompositor compositor(compositingMode);
		const SvgSuperSampler superSampler(subSamples);

		auto& bounds = polygon.GetBounds();

		const std::size_t startY = static_cast<std::size_t>(std::max(0.0, std::floor(bounds.GetMinY())));
		const std::size_t startX = static_cast<std::size_t>(std::max(0.0, std::floor(bounds.GetMinX())));
		const std::size_t width = std::min(_width, static_cast<std::size_t>(std::ceil(bounds.GetMaxX())));
		const std::size_t height = std::min(_height, static_cast<std::size_t>(std::ceil(bounds.GetMaxY())));

		//DoDrawPolygon(polygon, paint, fillRule, blender, compositor, superSampler, startY, startX, width, height);
		DrawPolygonMultiThreaded(polygon, paint, fillRule, blender, compositor, superSampler, startY, startX, width, height);
	}

	void SvgBackDrop::DrawPolygonMultiThreaded(const SvgPolygon& polygon, const SvgCalculatedPaint& paint,
			SvgFillRule& fillRule,
			const SvgBlender& blender, const SvgCompositor& compositor, const SvgSuperSampler& superSampler,
			const size_t startY, const size_t startX, const size_t width, const size_t height)
	{
		auto threadCount = std::min((std::size_t)std::thread::hardware_concurrency(), height);
		std::size_t linesPerThread = std::ceil(height / static_cast<double>(threadCount));

		std::vector<std::thread> threads;
		threads.reserve(threadCount);
		for (std::size_t t = 0, h = startY; t < threadCount; ++t, h += linesPerThread)
		{
			auto thisEndLine = std::min(h + linesPerThread, height);
			threads.emplace_back([&](std::size_t thisStartY, std::size_t thisEndY)
			{
				DoDrawPolygon(polygon, paint, fillRule, blender, compositor, superSampler, thisStartY, startX, width, thisEndY);
			}, h, thisEndLine);
		}

		for (auto& th: threads)
		{
			th.join();
		}
	}

	void SvgBackDrop::DoDrawPolygon(
			const SvgPolygon& polygon,
			const SvgCalculatedPaint& paint,
			SvgFillRule fillRule,
			const SvgBlender& blender,
			const SvgCompositor& compositor,
			const SvgSuperSampler& superSampler,
			const size_t startY,
			const size_t startX,
			const size_t width,
			const size_t height)
	{
		for (std::size_t y = startY; y < height; ++y)
		{
			for (std::size_t x = startX; x < width; ++x)
			{
				auto calculatedColor = superSampler.CalculatePixelColor(polygon, x, y, paint, fillRule);

				// See https://www.w3.org/TR/2015/CR-compositing-1-20150113/#generalformula
				auto& backdropColor = _colorData[y * _width + x];

				auto color = blender.Blend(calculatedColor, backdropColor);
				color = compositor.Composite(color, backdropColor);

				if (color.A() > 0.0)
				{
					// the color after these calculation is alpha pre-multiplied, so we need to un-premultiply
					backdropColor = SvgColor(color.R() / color.A(), color.G() / color.A(), color.B() / color.A(),
							color.A());
				}
			}
		}
	}

	void SvgBackDrop::Draw(const SvgBackDrop& other,
			std::size_t x,
			std::size_t y,
			double opacity,
			SvgBlendMode blendMode,
			SvgCompositingMode compositingMode)
	{
		if (x > _width || y > _height) return;

		const SvgBlender blender(blendMode);
		const SvgCompositor compositor(compositingMode);

		const auto startX = std::max(0ul, std::min(x, _width));
		const auto startY = std::max(0ul, std::min(y, _height));
		const auto width = std::min(_width, x + other.GetWidth());
		const auto height = std::min(_height, y + other.GetHeight());

		auto& colorData = other.GetColorData();
		const auto sourceWidth = other.GetWidth();

		//DoDrawOther(opacity, blender, compositor, startX, startY, width, height, 0, colorData, sourceWidth);
		DoDrawOtherMultiThreaded(opacity, blender, compositor, startX, startY, width, height, colorData, sourceWidth);
	}

	void
	SvgBackDrop::DoDrawOtherMultiThreaded(double opacity, const SvgBlender& blender, const SvgCompositor& compositor,
			const size_t startX, const size_t startY, const size_t width, const size_t height,
			const std::vector<SvgColor>& colorData, const size_t sourceWidth)
	{
		auto threadCount = std::min((std::size_t)std::thread::hardware_concurrency(), height);
		std::size_t linesPerThread = ceil(height / static_cast<double>(threadCount));

		std::vector<std::thread> threads;
		threads.reserve(threadCount);
		for (std::size_t t = 0, h = startY, sourceY = 0; t < threadCount; ++t, h += linesPerThread, sourceY += linesPerThread)
		{
			auto thisEndLine = std::min(h + linesPerThread, height);
			threads.emplace_back([&](std::size_t thisStartY, std::size_t thisEndY, std::size_t sourceY)
			{
				DoDrawOther(opacity, blender, compositor, startX, thisStartY, width, thisEndY, sourceY, colorData, sourceWidth);
			}, h, thisEndLine, sourceY);
		}

		for (auto& th: threads)
		{
			th.join();
		}
	}

	void SvgBackDrop::DoDrawOther(double opacity, const SvgBlender& blender, const SvgCompositor& compositor,
			const size_t startX,
			const size_t startY,
			const size_t width,
			const size_t height,
			size_t sourceY,
			const std::vector<SvgColor>& colorData, const size_t sourceWidth)
	{
		for (std::size_t targetY = startY; targetY < height; ++targetY, ++sourceY)
		{
			for (std::size_t targetX = startX, sourceX = 0; targetX < width; ++targetX, ++sourceX)
			{
				auto inputColor = colorData[sourceY * sourceWidth + sourceX].WithMultipliedAplha(opacity);
				auto& backdropColor = _colorData[targetY * _width + targetX];

				auto color = blender.Blend(inputColor, backdropColor);
				color = compositor.Composite(color, backdropColor);

				if (color.A() > 0.0)
				{
					// the color after these calculation is alpha pre-multiplied, so we need to un-premultiply
					backdropColor = SvgColor(color.R() / color.A(), color.G() / color.A(), color.B() / color.A(),
							color.A());
				}
			}
		}
	}

	SvgBackDrop::SvgBackDrop(std::size_t width, std::size_t height)
			: _width(width), _height(height)
	{
		_colorData.resize(width * height);
	}

	SvgBackDrop::SvgBackDrop(SvgBackDrop&& other) noexcept
			: _colorData(std::move(other._colorData)), _width(other._width), _height(other._height)
	{
	}

	SvgBackDrop& SvgBackDrop::operator=(SvgBackDrop&& other) noexcept
	{
		_height = other._height;
		_width = other._width;
		_colorData = std::move(other._colorData);
		return *this;
	}

	SvgBackDrop::SvgBackDrop()
			: _width(0), _height(0)
	{
	}

	size_t SvgBackDrop::GetWidth() const
	{
		return _width;
	}

	size_t SvgBackDrop::GetHeight() const
	{
		return _height;
	}
} // Elpida
