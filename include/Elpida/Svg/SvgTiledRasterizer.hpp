/*
 *  Copyright (c) 2026  Ioannis Panagiotopoulos
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
// Created by klapeto on 16/11/2026.
//

#ifndef ELPIDA_SVGTILEDRASTERIZER_HPP
#define ELPIDA_SVGTILEDRASTERIZER_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

namespace Elpida
{
	class SvgCalculatedDocument;
	class SvgCalculatedPaint;
	class SvgPolygon;
	enum class SvgFillRule;

	/**
	 * Scanline rasterizer with exact area coverage (signed-area accumulation, as in font-rs, stb_truetype and AGG).
	 * The image is rendered in TileSize x TileSize tiles: edges are binned per tile row once, every tile then
	 * accumulates the coverage of the shapes that overlap it, evaluates the paint once per covered pixel and
	 * composites source-over into 8-bit premultiplied RGBA. A tile and its accumulator stay in L1 even on old CPUs.
	 */
	class SvgTiledRasterizer
	{
	public:
		static constexpr int TileSize = 32;

		/**
		 * Renders the document.
		 * @param document The calculated document
		 * @param output Receives width * height * 4 bytes of premultiplied RGBA8, row major. Reused between calls.
		 */
		void Rasterize(const SvgCalculatedDocument& document, std::vector<std::uint8_t>& output);

		[[nodiscard]]
		std::size_t GetWidth() const
		{
			return _width;
		}

		[[nodiscard]]
		std::size_t GetHeight() const
		{
			return _height;
		}

	private:
		struct Edge
		{
			float x0, y0, x1, y1;    // y0 < y1, absolute pixel coordinates
			float direction;         // +1 / -1 winding direction
			float minX, maxX;
		};

		struct Item
		{
			const SvgCalculatedPaint* paint;
			float opacity;
			bool evenOdd;
			int pixelX0, pixelY0, pixelX1, pixelY1;    // pixel bounds clipped to the image, exclusive end
		};

		struct BandEntry
		{
			std::uint32_t item;
			std::uint32_t firstEdge;
			std::uint32_t edgeCount;
		};

		std::size_t _width = 0;
		std::size_t _height = 0;
		std::vector<Item> _items;
		std::vector<Edge> _edges;
		std::vector<std::vector<BandEntry>> _bands;
		std::vector<std::uint8_t> _tile;
		std::vector<float> _accumulator;
		std::vector<float> _coverage;

		void Collect(const SvgCalculatedDocument& document);
		void AddItem(const SvgPolygon& polygon, const SvgCalculatedPaint& paint, SvgFillRule fillRule, float opacity);
		void RenderTile(int tileX, int tileY);
	};
} // Elpida

#endif //ELPIDA_SVGTILEDRASTERIZER_HPP
