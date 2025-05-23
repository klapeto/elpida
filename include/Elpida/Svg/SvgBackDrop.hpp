//
// Created by klapeto on 6/3/2024.
//

#ifndef ELPIDA_SVG_SVGFRAMEBUFFER_HPP
#define ELPIDA_SVG_SVGFRAMEBUFFER_HPP

#include <cstddef>
#include <vector>

#include "SvgConfig.hpp"
#include "SvgBlendMode.hpp"
#include "SvgColor.hpp"
#include "SvgCompositingMode.hpp"
#include "SvgFillRule.hpp"
#include "SvgBlender.hpp"
#include "SvgCompositor.hpp"
#include "SvgSuperSampler.hpp"

namespace Elpida
{
	class SvgPolygon;

	class SvgCalculatedPaint;
	class SvgSuperSampler;

	class SvgBackDrop final
	{
	public:
		[[nodiscard]]
		std::size_t GetWidth() const;

		[[nodiscard]]
		std::size_t GetHeight() const;

		[[nodiscard]]
		const std::vector<SvgColor>& GetColorData() const
		{
			return _colorData;
		}

		void Draw(const SvgPolygon& polygon,
				const SvgCalculatedPaint& paint,
				const SvgSuperSampler& superSampler,
				SvgFillRule fillRule = SvgFillRule::NonZero,
				SvgBlendMode blendMode = SvgBlendMode::Normal,
				SvgCompositingMode compositingMode = SvgCompositingMode::SourceOver,
				SvgFloat opacity = 1.0);

		void DrawMultiThread(const SvgPolygon& polygon,
				const SvgCalculatedPaint& paint,
				const SvgSuperSampler& superSampler,
				SvgFillRule fillRule = SvgFillRule::NonZero,
				SvgBlendMode blendMode = SvgBlendMode::Normal,
				SvgCompositingMode compositingMode = SvgCompositingMode::SourceOver,
				SvgFloat opacity = 1.0);

		void Draw(const SvgBackDrop& other,
				std::size_t x,
				std::size_t y,
				SvgFloat opacity = 1.0,
				SvgBlendMode blendMode = SvgBlendMode::Normal,
				SvgCompositingMode compositingMode = SvgCompositingMode::SourceOver);

		void DrawMultiThread(const SvgBackDrop& other,
				std::size_t x,
				std::size_t y,
				SvgFloat opacity = 1.0,
				SvgBlendMode blendMode = SvgBlendMode::Normal,
				SvgCompositingMode compositingMode = SvgCompositingMode::SourceOver);

		void Draw(const SvgBackDrop& other,
				std::size_t x,
				std::size_t y,
				std::size_t width,
				std::size_t height,
				SvgFloat opacity = 1.0,
				SvgBlendMode blendMode = SvgBlendMode::Normal,
				SvgCompositingMode compositingMode = SvgCompositingMode::SourceOver);

		void Clear();

		SvgBackDrop();
		SvgBackDrop(std::size_t width, std::size_t height);
		SvgBackDrop(const SvgBackDrop&) = delete;
		SvgBackDrop& operator=(const SvgBackDrop&) = delete;
		SvgBackDrop(SvgBackDrop&&) noexcept;
		SvgBackDrop& operator=(SvgBackDrop&&) noexcept;
		~SvgBackDrop() = default;

	private:
		std::vector<SvgColor> _colorData;
		std::size_t _width;
		std::size_t _height;

		void DoDrawPolygon(const SvgPolygon& polygon,
				const SvgCalculatedPaint& paint,
				SvgFillRule fillRule,
				const Elpida::SvgBlender& blender,
				const Elpida::SvgCompositor& compositor,
				const Elpida::SvgSuperSampler& superSampler,
				std::size_t startY,
				std::size_t startX,
				std::size_t width,
				std::size_t height,
				SvgFloat opacity);

		void DrawPolygonMultiThreaded(const SvgPolygon& polygon,
				const SvgCalculatedPaint& paint,
				SvgFillRule& fillRule,
				const SvgBlender& blender,
				const SvgCompositor& compositor,
				const SvgSuperSampler& superSampler,
				size_t startY,
				size_t startX,
				size_t width,
				size_t height,
				SvgFloat opacity);

		void DoDrawOther(SvgFloat opacity,
				const SvgBlender& blender,
				const SvgCompositor& compositor,
				size_t startX,
				size_t startY,
				size_t endX,
				size_t endY,
				size_t otherX,
				size_t otherY,
				const std::vector<SvgColor>& colorData,
				size_t sourceWidth);

		void DoDrawOtherMultiThreaded(SvgFloat opacity,
				const SvgBlender& blender,
				const SvgCompositor& compositor,
				size_t startX,
				size_t startY,
				size_t width,
				size_t height,
				const std::vector<SvgColor>& colorData,
				size_t sourceWidth);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGFRAMEBUFFER_HPP
