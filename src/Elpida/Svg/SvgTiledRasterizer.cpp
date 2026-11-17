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

#include "Elpida/Svg/SvgTiledRasterizer.hpp"

#include "Elpida/Svg/SvgCalculatedDocument.hpp"
#include "Elpida/Svg/SvgCalculatedShape.hpp"
#include "Elpida/Svg/SvgCalculatedPaint.hpp"
#include "Elpida/Svg/SvgCalculatedFill.hpp"
#include "Elpida/Svg/SvgCalculatedStroke.hpp"
#include "Elpida/Svg/SvgPolygon.hpp"
#include "Elpida/Svg/SvgFillRule.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>

namespace Elpida
{
	static constexpr int Tile = SvgTiledRasterizer::TileSize;

	// Accumulator row stride. Contributions left of the tile fold into column 0, right of it into column Tile + 1,
	// which is never read.
	static constexpr int Stride = Tile + 2;

	static inline int Fold(int i)
	{
		return i < 0 ? 0 : (i > Tile + 1 ? Tile + 1 : i);
	}

	static inline unsigned Div255(unsigned v)
	{
		v += 128;
		return (v + (v >> 8)) >> 8;
	}

	// Source-over of one color with the given coverage into a premultiplied RGBA8 pixel.
	[[gnu::always_inline]] static inline void Composite(std::uint8_t* pixel, float r, float g, float b, float alpha,
			float coverage, float alphaScale)
	{
		const float a = alpha * coverage * alphaScale;
		const unsigned sa = (unsigned)(a * 255.0f + 0.5f);
		if (sa == 0) return;
		const unsigned sr = std::min(sa, (unsigned)(r * a * 255.0f + 0.5f));
		const unsigned sg = std::min(sa, (unsigned)(g * a * 255.0f + 0.5f));
		const unsigned sb = std::min(sa, (unsigned)(b * a * 255.0f + 0.5f));
		const unsigned inverse = 255 - sa;
		pixel[0] = (std::uint8_t)(sr + Div255(pixel[0] * inverse));
		pixel[1] = (std::uint8_t)(sg + Div255(pixel[1] * inverse));
		pixel[2] = (std::uint8_t)(sb + Div255(pixel[2] * inverse));
		pixel[3] = (std::uint8_t)(sa + Div255(pixel[3] * inverse));
	}

	// Pixels below this coverage are skipped
	static constexpr float MinCoverage = 1.0f / 1024.0f;

	void SvgTiledRasterizer::AddItem(const SvgPolygon& polygon, const SvgCalculatedPaint& paint, SvgFillRule fillRule,
			float opacity)
	{
		auto& edges = polygon.GetEdges();
		if (edges.empty()) return;

		float minX = INFINITY, minY = INFINITY, maxX = -INFINITY, maxY = -INFINITY;
		for (auto& edge : edges)
		{
			minX = std::min({ minX, (float)edge.GetA().GetX(), (float)edge.GetB().GetX() });
			maxX = std::max({ maxX, (float)edge.GetA().GetX(), (float)edge.GetB().GetX() });
			minY = std::min({ minY, (float)edge.GetA().GetY(), (float)edge.GetB().GetY() });
			maxY = std::max({ maxY, (float)edge.GetA().GetY(), (float)edge.GetB().GetY() });
		}

		Item item{};
		item.paint = &paint;
		item.opacity = opacity;
		item.evenOdd = fillRule == SvgFillRule::EvenOdd;
		item.pixelX0 = std::max(0, (int)std::floor(minX));
		item.pixelY0 = std::max(0, (int)std::floor(minY));
		item.pixelX1 = std::min((int)_width, (int)std::floor(maxX) + 1);
		item.pixelY1 = std::min((int)_height, (int)std::floor(maxY) + 1);
		if (item.pixelX0 >= item.pixelX1 || item.pixelY0 >= item.pixelY1) return;

		const auto itemIndex = (std::uint32_t)_items.size();
		_items.push_back(item);

		const int bandFirst = item.pixelY0 / Tile;
		const int bandLast = (item.pixelY1 - 1) / Tile;
		for (int band = bandFirst; band <= bandLast; ++band)
		{
			const float bandTop = (float)(band * Tile);
			const float bandBottom = (float)((band + 1) * Tile);
			BandEntry entry{ itemIndex, (std::uint32_t)_edges.size(), 0 };
			for (auto& edge : edges)
			{
				float x0 = edge.GetA().GetX(), y0 = edge.GetA().GetY();
				float x1 = edge.GetB().GetX(), y1 = edge.GetB().GetY();
				float direction = (float)edge.GetDirection();
				if (y0 == y1) continue;
				if (y0 > y1)
				{
					std::swap(x0, x1);
					std::swap(y0, y1);
					direction = -direction;
				}
				if (y1 <= bandTop || y0 >= bandBottom) continue;
				_edges.push_back({ x0, y0, x1, y1, direction, std::min(x0, x1), std::max(x0, x1) });
				entry.edgeCount++;
			}
			if (entry.edgeCount > 0) _bands[band].push_back(entry);
		}
	}

	void SvgTiledRasterizer::Collect(const SvgCalculatedDocument& document)
	{
		// Same paint order and opacity handling as SvgDirectRasterizer: fill, stroke, then the children.
		struct Walker
		{
			SvgTiledRasterizer& self;

			void Walk(const SvgCalculatedShape& shape)
			{
				auto& fill = shape.GetFill();
				if (fill.has_value())
				{
					self.AddItem(shape.GetFillPolygon(), *fill, fill->GetFillRule(), shape.GetOpacity());
				}
				auto& stroke = shape.GetStroke();
				if (stroke.has_value())
				{
					self.AddItem(shape.GetStrokePolygon(), *stroke, SvgFillRule::NonZero, shape.GetOpacity());
				}
				for (auto& child : shape.GetChildren())
				{
					Walk(child);
				}
			}
		};
		Walker{ *this }.Walk(document.GetRootShape());
	}

	// Signed-area accumulation of one edge, clipped to rows [rowBegin, rowEnd) of the tile at (tileX, tileY).
	static inline void DrawEdge(float* accumulator, float ex0, float ey0, float ex1, float ey1, float direction,
			float maxX, float tileX, float tileY, int rowBegin, int rowEnd)
	{
		float ya = std::max(ey0, tileY + (float)rowBegin);
		float yb = std::min(ey1, tileY + (float)rowEnd);
		if (ya >= yb) return;

		const float dxdy = (ex1 - ex0) / (ey1 - ey0);
		float x = ex0 + (ya - ey0) * dxdy - tileX;
		ya -= tileY;
		yb -= tileY;

		const int yFirst = (int)ya;
		const int yLast = (int)std::ceil(yb);

		if (maxX <= tileX)
		{
			// Entirely left of the tile: the whole winding enters at column 0.
			for (int y = yFirst; y < yLast; ++y)
			{
				const float dy = std::min((float)(y + 1), yb) - std::max((float)y, ya);
				accumulator[y * Stride] += dy * direction;
			}
			return;
		}

		for (int y = yFirst; y < yLast; ++y)
		{
			float* row = accumulator + y * Stride;
			const float dy = std::min((float)(y + 1), yb) - std::max((float)y, ya);
			const float xNext = x + dxdy * dy;
			const float d = dy * direction;
			const float xL = std::min(x, xNext), xR = std::max(x, xNext);
			const float xLFloor = std::floor(xL);
			const int xLi = (int)xLFloor;
			const float xRCeil = std::ceil(xR);
			const int xRi = (int)xRCeil;

			if (xRi <= xLi + 1)
			{
				// Within one pixel column
				const float xmf = 0.5f * (x + xNext) - xLFloor;
				row[Fold(xLi)] += d - d * xmf;
				row[Fold(xLi + 1)] += d * xmf;
			}
			else
			{
				const float s = 1.0f / (xR - xL);
				const float xLf = xL - xLFloor;
				const float a0 = 0.5f * s * (1.0f - xLf) * (1.0f - xLf);
				const float xRf = xR - xRCeil + 1.0f;
				const float am = 0.5f * s * xRf * xRf;
				row[Fold(xLi)] += d * a0;
				if (xRi == xLi + 2)
				{
					row[Fold(xLi + 1)] += d * (1.0f - a0 - am);
				}
				else
				{
					const float a1 = s * (1.5f - xLf);
					row[Fold(xLi + 1)] += d * (a1 - a0);
					int lo = xLi + 2, hi = xRi - 2;
					if (lo < 0)
					{
						const int n = std::min(hi, -1) - lo + 1;
						if (n > 0) row[0] += d * s * (float)n;
						lo = 0;
					}
					hi = std::min(hi, Tile);
					for (int xi = lo; xi <= hi; ++xi) row[xi] += d * s;
					const float a2 = a1 + (float)(xRi - xLi - 3) * s;
					row[Fold(xRi - 1)] += d * (1.0f - a2 - am);
				}
				row[Fold(xRi)] += d * am;
			}
			x = xNext;
		}
	}

	void SvgTiledRasterizer::RenderTile(int tileX, int tileY)
	{
		const int X0 = tileX * Tile, Y0 = tileY * Tile;
		const int tileWidth = std::min(Tile, (int)_width - X0);
		const int tileHeight = std::min(Tile, (int)_height - Y0);
		std::uint8_t* tile = _tile.data();
		float* accumulator = _accumulator.data();
		float* coverage = _coverage.data();
		std::memset(tile, 0, Tile * Tile * 4);

		for (auto& entry: _bands[tileY])
		{
			auto& item = _items[entry.item];
			if (item.pixelX1 <= X0 || item.pixelX0 >= X0 + tileWidth) continue;
			const int lx0 = std::max(item.pixelX0 - X0, 0), lx1 = std::min(item.pixelX1 - X0, tileWidth);
			const int ly0 = std::max(item.pixelY0 - Y0, 0), ly1 = std::min(item.pixelY1 - Y0, tileHeight);
			if (ly0 >= ly1) continue;

			for (int y = ly0; y < ly1; ++y) std::memset(accumulator + y * Stride, 0, Stride * sizeof(float));

			const float rightLimit = (float)(X0 + lx1);
			const Edge* edge = _edges.data() + entry.firstEdge;
			for (std::uint32_t i = 0; i < entry.edgeCount; ++i, ++edge)
			{
				if (edge->minX >= rightLimit) continue;
				DrawEdge(accumulator, edge->x0, edge->y0, edge->x1, edge->y1, edge->direction, edge->maxX, (float)X0,
						(float)Y0, ly0, ly1);
			}

			const auto& paint = *item.paint;
			const float alphaScale = item.opacity * (float)paint.Opacity();
			const bool solid = paint.IsSolidColor();
			const SvgColor solidColor = solid ? paint.CalculateColor(SvgPoint()) : SvgColor();
			for (int y = ly0; y < ly1; ++y)
			{
				// Coverage of the row: running sum of the accumulator with the fill rule applied. Kept free of
				// calls so that it compiles to the same loop under every calling convention.
				const float* row = accumulator + y * Stride;
				float sum = 0.0f;
				// Columns left of lx0 only ever receive folded contributions when lx0 == 0
				for (int x = 0; x < lx0; ++x) sum += row[x];
				if (item.evenOdd)
				{
					for (int x = lx0; x < lx1; ++x)
					{
						sum += row[x];
						float c = std::fabs(sum);
						c -= 2.0f * (float)(int)(c * 0.5f);    // c >= 0, so truncation is floor
						coverage[x] = c > 1.0f ? 2.0f - c : c;
					}
				}
				else
				{
					for (int x = lx0; x < lx1; ++x)
					{
						sum += row[x];
						coverage[x] = std::min(std::fabs(sum), 1.0f);
					}
				}

				std::uint8_t* pixels = tile + y * Tile * 4;
				if (solid)
				{
					const float r = solidColor.R(), g = solidColor.G(), b = solidColor.B(), a = solidColor.A();
					for (int x = lx0; x < lx1; ++x)
					{
						if (coverage[x] < MinCoverage) continue;
						Composite(pixels + x * 4, r, g, b, a, coverage[x], alphaScale);
					}
				}
				else
				{
					for (int x = lx0; x < lx1; ++x)
					{
						if (coverage[x] < MinCoverage) continue;
						const SvgColor color = paint.CalculateColor(
								SvgPoint((float)(X0 + x) + 0.5f, (float)(Y0 + y) + 0.5f));
						Composite(pixels + x * 4, color.R(), color.G(), color.B(), color.A(), coverage[x],
								alphaScale);
					}
				}
			}
		}
	}

	void SvgTiledRasterizer::Rasterize(const SvgCalculatedDocument& document, std::vector<std::uint8_t>& output)
	{
		auto& viewPort = document.GetViewPort();
		_width = (std::size_t)std::ceil(viewPort.GetWidth());
		_height = (std::size_t)std::ceil(viewPort.GetHeight());
		const int bandCount = (int)((_height + Tile - 1) / Tile);
		const int tileColumns = (int)((_width + Tile - 1) / Tile);

		// Keep the allocations of previous calls
		_items.clear();
		_edges.clear();
		_bands.resize(bandCount);
		for (auto& band: _bands) band.clear();
		_tile.resize(Tile * Tile * 4);
		_accumulator.resize(Tile * Stride);
		_coverage.resize(Tile);

		Collect(document);

		// Every pixel is written by its tile, so the output needs no clearing.
		output.resize(_width * _height * 4);

		for (int tileY = 0; tileY < bandCount; ++tileY)
		{
			for (int tileX = 0; tileX < tileColumns; ++tileX)
			{
				RenderTile(tileX, tileY);
				const int X0 = tileX * Tile, Y0 = tileY * Tile;
				const int tileWidth = std::min(Tile, (int)_width - X0);
				const int tileHeight = std::min(Tile, (int)_height - Y0);
				for (int y = 0; y < tileHeight; ++y)
				{
					std::memcpy(output.data() + ((std::size_t)(Y0 + y) * _width + X0) * 4, _tile.data() + y * Tile * 4,
							tileWidth * 4);
				}
			}
		}
	}
} // Elpida
