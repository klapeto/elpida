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
	class SvgCalculatedDocument;
	class SvgViewBox;
	class SvgCalculatedShape;
	class SvgTransform;
	class ThreadPool;

	class SvgRasterizer
	{
	public:
		SvgBackDrop Rasterize(SvgCalculatedDocument& document, std::size_t subSamples = 16);
		SvgBackDrop RasterizeMultiThreaded(SvgCalculatedDocument& document, ThreadPool& threadPool, std::size_t subSamples = 16);
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
			std::size_t GetX() const
			{
				return _x;
			}

			[[nodiscard]]
			std::size_t GetY() const
			{
				return _y;
			}

			[[nodiscard]]
			bool IsValid() const
			{
				return _backdrop.GetWidth() > 0;
			}

			RasterizedShape() = default;
			RasterizedShape(SvgBackDrop&& backDrop,  std::size_t x, std::size_t y)
					: _backdrop(std::move(backDrop)), _x(x), _y(y)
			{

			}

			RasterizedShape(const RasterizedShape&) noexcept = delete;
			RasterizedShape& operator=(const RasterizedShape&) = delete;
			RasterizedShape(RasterizedShape&&) noexcept = default;
			RasterizedShape& operator=(RasterizedShape&&) noexcept = default;
		private:
			SvgBackDrop _backdrop;
			std::size_t _x = 0;
			std::size_t _y = 0;
		};
		void RasterizeRootShape(SvgBackDrop& backDrop, SvgCalculatedShape& shape, std::size_t subSamples);
		static RasterizedShape RasterizeShape(SvgCalculatedShape &shape, std::size_t subSamples);
		static std::future<RasterizedShape> RasterizedSelfShape(SvgCalculatedShape &shape, std::size_t subSamples);
		static RasterizedShape RasterizePolygon(SvgPolygon polygon, SvgCalculatedPaint& paint, SvgFillRule fillRule, std::size_t subSamples);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZER_HPP
