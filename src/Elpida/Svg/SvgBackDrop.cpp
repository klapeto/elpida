/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 6/3/2024.
//

#include "Elpida/Svg/SvgBackDrop.hpp"
#include "Elpida/Svg/SvgBlender.hpp"
#include "Elpida/Svg/SvgCompositor.hpp"
#include "Elpida/Svg/SvgSuperSampler.hpp"
#include "Elpida/Svg/SvgPolygon.hpp"
#include "Elpida/Svg/SvgCalculatedPaint.hpp"

#include <cmath>
#include <algorithm>
#include <cstring>
#include <future>

namespace Elpida
{
	void SvgBackDrop::Draw(const SvgPolygon& polygon,
			const SvgCalculatedPaint& paint,
			const SvgSuperSampler& superSampler,
			SvgFillRule fillRule,
			SvgBlendMode blendMode,
			SvgCompositingMode compositingMode,
			SvgFloat opacity)
	{
		const SvgBlender blender(blendMode);
		const SvgCompositor compositor(compositingMode);

		auto& bounds = polygon.GetBounds();

		const std::size_t startY = static_cast<std::size_t>(std::max(SvgFloat(0.0), std::floor(bounds.GetMinY())));
		const std::size_t startX = static_cast<std::size_t>(std::max(SvgFloat(0.0), std::floor(bounds.GetMinX())));

		// We add + 0.5 because here the (0,0) is at the "center" of the pixel (0,0) where in Svg coordinates it is at top left
		// with this we ensure we have enough width for this case. See the super sample that starts eg. at x + 0.5
		const std::size_t width = std::min(_width, static_cast<std::size_t>(std::ceil(bounds.GetWidth() + SvgFloat(0.5))));
		const std::size_t height = std::min(_height, static_cast<std::size_t>(std::ceil(bounds.GetHeight() + SvgFloat(0.5))));

		DoDrawPolygon(polygon, paint, fillRule, blender, compositor, superSampler, startY, startX, width, height, opacity);
	}

	void
	SvgBackDrop::DrawMultiThread(const SvgPolygon& polygon,
			const SvgCalculatedPaint& paint,
			const SvgSuperSampler& superSampler,
			SvgFillRule fillRule, SvgBlendMode blendMode, SvgCompositingMode compositingMode,
			SvgFloat opacity)
	{
		const SvgBlender blender(blendMode);
		const SvgCompositor compositor(compositingMode);

		auto& bounds = polygon.GetBounds();

		const std::size_t startY = static_cast<std::size_t>(std::max(SvgFloat(0.0), std::floor(bounds.GetMinY())));
		const std::size_t startX = static_cast<std::size_t>(std::max(SvgFloat(0.0), std::floor(bounds.GetMinX())));

		// We add + 0.5 because here the (0,0) is at the "center" of the pixel (0,0) where in Svg coordinates it is at top left
		// with this we ensure we have enough width for this case. See the super sample that starts eg. at x + 0.5
		const std::size_t width = std::min(_width, static_cast<std::size_t>(std::ceil(bounds.GetWidth() + SvgFloat(0.5))));
		const std::size_t height = std::min(_height, static_cast<std::size_t>(std::ceil(bounds.GetHeight() + SvgFloat(0.5))));

		DrawPolygonMultiThreaded(polygon, paint, fillRule, blender, compositor, superSampler, startY, startX, width, height, opacity);
	}

	void
	SvgBackDrop::DoDrawOtherMultiThreaded(SvgFloat opacity,
			const SvgBlender& blender,
			const SvgCompositor& compositor,
			const size_t startX,
			const size_t startY,
			const size_t width,
			const size_t height,
			const std::vector<SvgColor>& colorData,
			const size_t sourceWidth)
	{
		auto threadCount = std::min(static_cast<std::size_t>(std::thread::hardware_concurrency()), height);
		std::size_t linesPerThread = std::ceil(height / static_cast<SvgFloat>(threadCount));

		std::vector<std::future<void>> threads;
		threads.reserve(threadCount);
		for (std::size_t t = 0, h = startY, sourceY = 0; t < threadCount; ++t, h += linesPerThread, sourceY += linesPerThread)
		{
			auto thisEndLine = std::min(h + linesPerThread, height);
			threads.emplace_back(std::async([&, thisStartY = h, thisEndY = thisEndLine, sourceY = sourceY]
			{
				DoDrawOther(opacity, blender, compositor, startX, thisStartY, width, thisEndY,0, sourceY, colorData, sourceWidth);
			}));
		}

		for (auto& th: threads)
		{
			th.get();
		}
	}

	void SvgBackDrop::DrawPolygonMultiThreaded(const SvgPolygon& polygon,
			const SvgCalculatedPaint& paint,
			SvgFillRule& fillRule,
			const SvgBlender& blender,
			const SvgCompositor& compositor,
			const SvgSuperSampler& superSampler,
			const size_t startY,
			const size_t startX,
			const size_t width,
			const size_t height,
			SvgFloat opacity)
	{
		auto threadCount = std::min(static_cast<std::size_t>(std::thread::hardware_concurrency()), height);
		std::size_t linesPerThread = std::ceil(height / static_cast<SvgFloat>(threadCount));

		std::vector<std::future<void>> threads;
		threads.reserve(threadCount);
		for (std::size_t t = 0, h = startY; t < threadCount; ++t, h += linesPerThread)
		{
			auto thisEndLine = std::min(h + linesPerThread, height);
			threads.push_back(std::async([&, thisStartY = h, thisEndY = thisEndLine]()
			{
				DoDrawPolygon(polygon, paint, fillRule, blender, compositor, superSampler, thisStartY, startX, width, thisEndY - thisStartY, opacity);
			}));
		}

		for (auto& th: threads)
		{
			th.get();
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
			const size_t height,
			SvgFloat opacity)
	{
		auto endY = std::min(_height, startY + height);
		auto endX = std::min(_width, startX + width);
		for (std::size_t y = startY; y < endY; ++y)
		{
			for (std::size_t x = startX; x < endX; ++x)
			{
				auto calculatedColor = superSampler.CalculatePixelColor(polygon, x, y, paint, fillRule).WithMultipliedAplha(opacity);

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
			SvgFloat opacity,
			SvgBlendMode blendMode,
			SvgCompositingMode compositingMode)
	{
		if (x > _width || y > _height) return;

		const SvgBlender blender(blendMode);
		const SvgCompositor compositor(compositingMode);

		const auto startX = std::max(static_cast<std::size_t>(0), std::min(x, _width));
		const auto startY = std::max(static_cast<std::size_t>(0), std::min(y, _height));
		const auto endX = std::min(_width, x + other.GetWidth());
		const auto endY = std::min(_height, y + other.GetHeight());

		auto& colorData = other.GetColorData();
		const auto sourceWidth = other.GetWidth();

		DoDrawOther(opacity, blender, compositor, startX, startY, endX, endY, 0, 0, colorData, sourceWidth);
	}

	void SvgBackDrop::DoDrawOther(SvgFloat opacity, const SvgBlender& blender, const SvgCompositor& compositor,
			const size_t startX,
			const size_t startY,
			const size_t endX,
			const size_t endY,
			size_t otherX,
			size_t otherY,
			const std::vector<SvgColor>& colorData,
			const size_t sourceWidth)
	{
		for (std::size_t targetY = startY; targetY < endY; ++targetY, ++otherY)
		{
			for (std::size_t targetX = startX, sourceX = otherX; targetX < endX; ++targetX, ++sourceX)
			{
				auto inputColor = colorData[otherY * sourceWidth + sourceX].WithMultipliedAplha(opacity);
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

	SvgBackDrop::SvgBackDrop()
			:_width(0), _height(0)
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

	void SvgBackDrop::Clear()
	{
		//TODO: Optimize
		for (auto& i : _colorData)
		{
			i = {};
		}
	}

	void SvgBackDrop::Draw(const SvgBackDrop& other,
			std::size_t x,
			std::size_t y,
			std::size_t width,
			std::size_t height,
			SvgFloat opacity,
			SvgBlendMode blendMode,
			SvgCompositingMode compositingMode)
	{
		if (x > _width || y > _height) return;

		const SvgBlender blender(blendMode);
		const SvgCompositor compositor(compositingMode);

		const auto startX = std::max(static_cast<std::size_t>(0), std::min(x, _width));
		const auto startY = std::max(static_cast<std::size_t>(0), std::min(y, _height));
		const auto endX = std::min(_width, x + width);	// +1 Inclusive pixel because of the internal checks do not include the last
		const auto endY = std::min(_height, y + height);

		auto& colorData = other.GetColorData();
		const auto sourceWidth = other.GetWidth();

		DoDrawOther(opacity, blender, compositor, startX, startY, endX, endY, startX, startY, colorData, sourceWidth);
	}


	void SvgBackDrop::DrawMultiThread(const SvgBackDrop& other,
			std::size_t x,
			std::size_t y,
			SvgFloat opacity,
			SvgBlendMode blendMode, SvgCompositingMode compositingMode)
	{
		if (x > _width || y > _height) return;

		const SvgBlender blender(blendMode);
		const SvgCompositor compositor(compositingMode);

		const auto startX = std::max(static_cast<std::size_t>(0), std::min(x, _width));
		const auto startY = std::max(static_cast<std::size_t>(0), std::min(y, _height));
		const auto endX = std::min(_width, x + other.GetWidth());
		const auto endY = std::min(_height, y + other.GetHeight());

		auto& colorData = other.GetColorData();
		const auto sourceWidth = other.GetWidth();

		DoDrawOtherMultiThreaded(opacity, blender, compositor, startX, startY, endX, endY, colorData, sourceWidth);
	}
} // Elpida
