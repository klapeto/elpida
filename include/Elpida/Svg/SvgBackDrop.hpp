//
// Created by klapeto on 6/3/2024.
//

#ifndef ELPIDA_SVG_SVGFRAMEBUFFER_HPP
#define ELPIDA_SVG_SVGFRAMEBUFFER_HPP

#include <cstddef>
#include <vector>

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

	class SvgBackDrop final
	{
	public:
		[[nodiscard]]
		std::size_t GetWidth() const;

		[[nodiscard]]
		std::size_t GetHeight() const;

		[[nodiscard]]
		const std::vector<SvgColor> &GetColorData() const
		{
			return _colorData;
		}

		void Draw(const SvgPolygon &polygon,
		          const SvgCalculatedPaint &paint,
		          SvgFillRule fillRule = SvgFillRule::NonZero,
		          SvgBlendMode blendMode = SvgBlendMode::Normal,
		          SvgCompositingMode compositingMode = SvgCompositingMode::SourceOver,
		          std::size_t subSamples = 32);

		void Draw(const SvgBackDrop &other,
		          std::size_t x,
		          std::size_t y,
		          double opacity = 1.0,
		          SvgBlendMode blendMode = SvgBlendMode::Normal,
		          SvgCompositingMode compositingMode = SvgCompositingMode::SourceOver);

		SvgBackDrop();
		SvgBackDrop(std::size_t width, std::size_t height);
		SvgBackDrop(const SvgBackDrop &) = delete;
		SvgBackDrop &operator=(const SvgBackDrop &) = delete;
		SvgBackDrop(SvgBackDrop &&) noexcept;
		SvgBackDrop &operator=(SvgBackDrop &&) noexcept;
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
				std::size_t height);
		void DrawPolygonMultiThreaded(const SvgPolygon& polygon, const SvgCalculatedPaint& paint, SvgFillRule& fillRule,
				const SvgBlender& blender, const SvgCompositor& compositor, const SvgSuperSampler& superSampler,
				const size_t startY, const size_t startX, const size_t width, const size_t height);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGFRAMEBUFFER_HPP
