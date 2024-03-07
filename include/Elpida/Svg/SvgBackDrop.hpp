//
// Created by klapeto on 6/3/2024.
//

#ifndef ELPIDA_SVG_SVGFRAMEBUFFER_HPP
#define ELPIDA_SVG_SVGFRAMEBUFFER_HPP

#include <cstddef>
#include <memory>
#include <vector>

#include "Elpida/Svg/SvgBlendMode.hpp"
#include "SvgColor.hpp"
#include "SvgCompositingMode.hpp"

namespace Elpida
{
	class SvgPolygon;

	class SvgRasterizerPaint;

	class SvgBackDrop final
	{
	public:

		[[nodiscard]]
		const std::vector<SvgColor>& GetColorData() const
		{
			return _colorData;
		}

		void Draw(const SvgPolygon& polygon, const SvgRasterizerPaint& paint,
				SvgBlendMode blendMode = SvgBlendMode::Normal,
				SvgCompositingMode compositingMode = SvgCompositingMode::SourceOver,
				std::size_t subSamples = 1);

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
	};
} // Elpida

#endif //ELPIDA_SVG_SVGFRAMEBUFFER_HPP
