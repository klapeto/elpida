//
// Created by klapeto on 6/3/2024.
//

#include "Elpida/Svg/SvgBackDrop.hpp"
#include "Elpida/Svg/SvgBlender.hpp"
#include "Elpida/Svg/SvgCompositor.hpp"
#include "Elpida/Svg/SvgSuperSampler.hpp"

#include <cmath>
#include <cstring>
#include <Elpida/Svg/SvgPolygon.hpp>
#include <Elpida/Svg/SvgCalculatedPaint.hpp>

namespace Elpida
{
	void SvgBackDrop::Draw(const SvgPolygon &polygon,
	                       const SvgCalculatedPaint &paint,
	                       double opacity,
	                       SvgFillRule fillRule,
	                       SvgBlendMode blendMode,
	                       SvgCompositingMode compositingMode,
	                       const std::size_t subSamples)
	{
		const SvgBlender blender(blendMode);
		const SvgCompositor compositor(compositingMode);
		const SvgSuperSampler superSampler(subSamples);

		auto &bounds = polygon.GetBounds();
		const std::size_t startY = std::floor(std::max(0.0, bounds.GetMinY()));
		const std::size_t startX = std::floor(std::max(0.0, bounds.GetMinX()));
		const std::size_t width = std::ceil(std::min(static_cast<double>(_width), bounds.GetWidth() + startX));
		const std::size_t height = std::ceil(std::min(static_cast<double>(_height), bounds.GetHeight() + startY));

		for (std::size_t y = startY; y < height; ++y)
		{
			for (std::size_t x = startX; x < width; ++x)
			{
				auto calculatedColor = superSampler.CalculatePixelColor(polygon, x + 0.5, y + 0.5, paint, fillRule);

				// See https://www.w3.org/TR/2015/CR-compositing-1-20150113/#generalformula
				auto &backdropColor = _colorData[y * _width + x];
				const auto as = calculatedColor.A() * opacity;
				const auto ab = backdropColor.A();

				auto rCs = blender.Blend(calculatedColor.R(), backdropColor.R(), ab);
				auto gCs = blender.Blend(calculatedColor.G(), backdropColor.G(), ab);
				auto bCs = blender.Blend(calculatedColor.B(), backdropColor.B(), ab);

				rCs = compositor.Composite(as, rCs, ab, backdropColor.R());
				gCs = compositor.Composite(as, gCs, ab, backdropColor.G());
				bCs = compositor.Composite(as, bCs, ab, backdropColor.B());

				auto ao = as + ab * (1.0 - as);

				if (ao <= 0.0)
				{
					// This is to avoid division by 0
					backdropColor = SvgColor(rCs, gCs, bCs, ao);
				}
				else
				{
					// the color after these calculation is alpha pre-multiplied, so we need to un-premultiply
					backdropColor = SvgColor(rCs / ao, gCs / ao, bCs / ao, ao);
				}
			}
		}
	}

	void SvgBackDrop::Draw(const SvgBackDrop &other,
	                       std::size_t x,
	                       std::size_t y,
	                       double opacity,
	                       SvgBlendMode blendMode,
	                       SvgCompositingMode compositingMode)
	{
		if (x > _width || y > _width) return;

		const SvgBlender blender(blendMode);
		const SvgCompositor compositor(compositingMode);

		const auto startX = std::min(x, _width);
		const auto startY = std::min(y, _height);
		const auto width = std::min(_width, other.GetWidth() + x);
		const auto height = std::min(_height, other.GetHeight() + y);

		auto& colorData = other.GetColorData();
		const auto sourceWidth = other.GetWidth();

		for (std::size_t targetY = startY, sourceY = 0; targetY < height; ++targetY, ++sourceY)
		{
			for (std::size_t targetX = startX, sourceX = 0; targetX < width; ++targetX, ++sourceX)
			{
				auto& inputColor = colorData[sourceY * sourceWidth + sourceX];
				auto& backdropColor = _colorData[targetY * _width + targetX];
				const auto as = inputColor.A() * opacity;
				const auto ab = backdropColor.A();

				auto rCs = blender.Blend(inputColor.R(), backdropColor.R(), ab);
				auto gCs = blender.Blend(inputColor.G(), backdropColor.G(), ab);
				auto bCs = blender.Blend(inputColor.B(), backdropColor.B(), ab);

				rCs = compositor.Composite(as, rCs, ab, backdropColor.R());
				gCs = compositor.Composite(as, gCs, ab, backdropColor.G());
				bCs = compositor.Composite(as, bCs, ab, backdropColor.B());

				auto ao = as + ab * (1.0 - as);

				if (ao <= 0.0)
				{
					// This is to avoid division by 0
					backdropColor = SvgColor(rCs, gCs, bCs, ao);
				}
				else
				{
					// the color after these calculation is alpha pre-multiplied, so we need to un-premultiply
					backdropColor = SvgColor(rCs / ao, gCs / ao, bCs / ao, ao);
				}
			}
		}
	}

	SvgBackDrop::SvgBackDrop(std::size_t width, std::size_t height)
		: _width(width), _height(height)
	{
		_colorData.resize(width * height);
	}

	SvgBackDrop::SvgBackDrop(SvgBackDrop &&other) noexcept
		: _colorData(std::move(other._colorData)), _width(other._width), _height(other._height)
	{
	}

	SvgBackDrop &SvgBackDrop::operator=(SvgBackDrop &&other) noexcept
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
