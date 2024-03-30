//
// Created by klapeto on 24/12/2023.
//

#ifndef ELPIDA_SVG_SVGRASTERIZER_HPP
#define ELPIDA_SVG_SVGRASTERIZER_HPP

#include <cstddef>
#include <future>

#include "Elpida/Svg/SvgBackDrop.hpp"
#include "Elpida/Svg/SvgTransform.hpp"
#include "Elpida/Svg/SvgBounds.hpp"

namespace Elpida
{
	class SvgDocument;
	class SvgViewBox;
	class SvgCalculatedShape;
	class SvgTransform;

	class SvgRasterizer
	{
	public:
		SvgBackDrop Rasterize(const SvgDocument& document, double scale = 1.0, std::size_t subSamples = 16);
	private:
		class RasterizedShape final
		{
		public:
			[[nodiscard]]
			const SvgBackDrop& GetBackdrop() const
			{
				return _backdrop;
			}

			[[nodiscard]]
			const SvgBounds& GetActualBounds() const
			{
				return _actualBounds;
			}

			RasterizedShape() = default;
			RasterizedShape(SvgBackDrop&& backDrop, const SvgBounds& actualBounds)
					: _backdrop(std::move(backDrop)), _actualBounds(actualBounds)
			{

			}

			RasterizedShape(const RasterizedShape&) noexcept = delete;
			RasterizedShape& operator=(const RasterizedShape&) = delete;
			RasterizedShape(RasterizedShape&&) noexcept = default;
			RasterizedShape& operator=(RasterizedShape&&) noexcept = default;
		private:
			SvgBackDrop _backdrop;
			SvgBounds _actualBounds;
		};
		void RasterizeRootShape(SvgBackDrop& backDrop, SvgCalculatedShape& shape, std::size_t subSamples);
		static RasterizedShape RasterizeShape(SvgCalculatedShape &shape, std::size_t subSamples);
		static std::future<RasterizedShape> RasterizedSelfShape(SvgCalculatedShape &shape, std::size_t subSamples);
		static RasterizedShape RasterizePolygon(SvgPolygon polygon, SvgCalculatedPaint& paint, SvgFillRule fillRule, std::size_t subSamples);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZER_HPP
