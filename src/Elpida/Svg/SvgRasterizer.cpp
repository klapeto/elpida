//
// Created by klapeto on 24/12/2023.
//

#include "Elpida/Svg/SvgRasterizer.hpp"

#include <algorithm>
#include <iostream>
#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgGradient.hpp>
#include <Elpida/Svg/SvgPath.hpp>
#include <Elpida/Svg/SvgPaint.hpp>
#include <Elpida/Svg/SvgRadialGradient.hpp>
#include <Elpida/Svg/SvgShape.hpp>

#include <cstring>
#include <list>
#include <Elpida/Svg/SvgLinearGradient.hpp>

namespace Elpida
{
	struct Edge
	{
		double x0, y0, x1, y1;
		int direction;
	};

	enum PaintType
	{
		PAINT_UNDEF = -1,
		PAINT_NONE = 0,
		PAINT_COLOR = 1,
		PAINT_LINEAR_GRADIENT = 2,
		PAINT_RADIAL_GRADIENT = 3
	};

	enum PointFlags
	{
		NONE = 0,
		CORNER = 0x01,
		BEVEL = 0x02,
		LEFT = 0x04
	};

	struct Paint
	{
		PaintType type;
		SvgSpreadType spread;
		SvgTransform transform;
		unsigned int colors[256];
	};

	struct RasterizerPoint
	{
		double x, y;
		double dx, dy;
		double len;
		double dmx, dmy;
		PointFlags flags;
	};

	struct ActiveEdge
	{
		int x, dx;
		double ey;
		int dir;

		bool operator <(const ActiveEdge& other) const
		{
			return x < other.x;
		}
	};

	static constexpr double tessTol = 0.25;
	static constexpr double distTol = 0.01;

	static constexpr int FixShift = 10;
	static constexpr int Fix = 1 << FixShift;
	static constexpr int FixMask = Fix - 1;

	static double AbsF(const double x) { return x < 0 ? -x : x; }
	static double RoundF(const double x) { return (x >= 0) ? floor(x + 0.5) : ceil(x - 0.5); }

	static int PointEquals(const double x1, const double y1, const double x2, const double y2)
	{
		const double dx = x2 - x1;
		const double dy = y2 - y1;
		return dx * dx + dy * dy < distTol * distTol;
	}

	static int PointEquals(const RasterizerPoint& a, const RasterizerPoint& b)
	{
		return PointEquals(a.x, a.y, b.x, b.y);
	}

	static int Div255(const int x)
	{
		return ((x + 1) * 257) >> 16;
	}

	static double Clamp(const double value, const double min, const double max)
	{
		return value < min ? min : (value > max ? max : value);
	}

	static unsigned int RGBA(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
	{
		return static_cast<unsigned int>(r)
			| (static_cast<unsigned int>(g) << 8)
			| (static_cast<unsigned int>(b) << 16)
			| (static_cast<unsigned int>(a) << 24);
	}

	static unsigned int ShiftColorToColor(const unsigned int colorA, const unsigned int colorB, const double u)
	{
		const int iu = static_cast<int>(Clamp(u, 0.0, 1.0) * 256.0);
		const int r = (((colorA) & 0xff) * (256 - iu) + (((colorB) & 0xff) * iu)) >> 8;
		const int g = (((colorA >> 8) & 0xff) * (256 - iu) + (((colorB >> 8) & 0xff) * iu)) >> 8;
		const int b = (((colorA >> 16) & 0xff) * (256 - iu) + (((colorB >> 16) & 0xff) * iu)) >> 8;
		const int a = (((colorA >> 24) & 0xff) * (256 - iu) + (((colorB >> 24) & 0xff) * iu)) >> 8;
		return RGBA(
			static_cast<unsigned char>(r),
			static_cast<unsigned char>(g),
			static_cast<unsigned char>(b),
			static_cast<unsigned char>(a));
	}

	static unsigned int ApplyOpacity(const unsigned int color, const double opacity)
	{
		const int iu = static_cast<int>(Clamp(opacity, 0.0, 1.0) * 256.0);
		const int r = (color) & 0xff;
		const int g = (color >> 8) & 0xff;
		const int b = (color >> 16) & 0xff;
		const int a = (((color >> 24) & 0xff) * iu) >> 8;
		return RGBA(
			static_cast<unsigned char>(r),
			static_cast<unsigned char>(g),
			static_cast<unsigned char>(b),
			static_cast<unsigned char>(a));
	}

	static void AddPoint(std::vector<RasterizerPoint>& points, const SvgPoint& point, PointFlags flags)
	{
		if (!points.empty())
		{
			auto& lastPoint = points.back();
			if (PointEquals(lastPoint.x, lastPoint.y, point.GetX(), point.GetY()))
			{
				lastPoint.flags = static_cast<PointFlags>(static_cast<unsigned int>(lastPoint.flags) | static_cast<
					unsigned int>(flags));
				return;
			}
		}

		points.push_back({
			.x = point.GetX(),
			.y = point.GetY(),
			.dx = 0,
			.dy = 0,
			.len = 0,
			.dmx = 0,
			.dmy = 0,
			.flags = PointFlags::NONE
		});
	}

	static void FlattenCubicBez(std::vector<RasterizerPoint>& points,
	                            const SvgPoint& p1,
	                            const SvgPoint& p2,
	                            const SvgPoint& p3,
	                            const SvgPoint& p4,
	                            const int level,
	                            const PointFlags type)
	{
		if (level > 10) return;

		const double dx = p4.GetX() - p1.GetX();
		const double dy = p4.GetY() - p1.GetY();
		const double d2 = AbsF(((p2.GetX() - p4.GetX()) * dy) - ((p2.GetY() - p4.GetY()) * dx));
		const double d3 = AbsF(((p3.GetX() - p4.GetX()) * dy) - ((p3.GetY() - p4.GetY()) * dx));

		if ((d2 + d3) * (d2 + d3) < tessTol * (dx * dx + dy * dy))
		{
			AddPoint(points, p4, type);
			return;
		}

		const auto p1p2 = (p1 + p2) * 0.5;
		const auto p2p3 = (p2 + p3) * 0.5;
		const auto p3p4 = (p3 + p4) * 0.5;
		const auto p1p2p3 = (p1p2 + p2p3) * 0.5;
		const auto p2p3p4 = (p2p3 + p3p4) * 0.5;
		const auto p1p2p3p4 = (p1p2p3 + p2p3p4) * 0.5;

		FlattenCubicBez(points, p1, p1p2, p1p2p3, p1p2p3p4, level + 1, PointFlags::NONE);
		FlattenCubicBez(points, p1p2p3p4, p2p3p4, p3p4, p4, level + 1, type);
	}

	static void AddEdge(std::vector<Edge>& edges, const double x0, const double y0, const double x1, const double y1)
	{
		// Skip horizontal edges
		if (y0 == y1)
		{
			return;
		}

		if (y0 < y1)
		{
			edges.push_back({
				.x0 = x0,
				.y0 = y0,
				.x1 = x1,
				.y1 = y1,
				.direction = 1,
			});
		}
		else
		{
			edges.push_back({
				.x0 = x1,
				.y0 = y1,
				.x1 = x0,
				.y1 = y0,
				.direction = -1
			});
		}
	}

	static void UnpremultiplyAlpha(unsigned char* image, std::size_t w, std::size_t h, std::size_t stride)
	{
		// Unpremultiply
		for (std::size_t y = 0; y < h; y++)
		{
			unsigned char* row = &image[y * stride];
			for (std::size_t x = 0; x < w; x++)
			{
				const int r = row[0];
				const int g = row[1];
				const int b = row[2];
				const int a = row[3];
				if (a != 0)
				{
					row[0] = static_cast<unsigned char>(r * 255 / a);
					row[1] = static_cast<unsigned char>(g * 255 / a);
					row[2] = static_cast<unsigned char>(b * 255 / a);
				}
				row += 4;
			}
		}

		// Defringe
		for (std::size_t y = 0; y < h; y++)
		{
			unsigned char* row = &image[y * stride];
			for (std::size_t x = 0; x < w; x++)
			{
				const int a = row[3];
				if (a == 0)
				{
					int n = 0;
					int b = 0;
					int g = 0;
					int r = 0;
					if (x - 1 > 0 && row[-1] != 0)
					{
						r += row[-4];
						g += row[-3];
						b += row[-2];
						n++;
					}
					if (x + 1 < w && row[7] != 0)
					{
						r += row[4];
						g += row[5];
						b += row[6];
						n++;
					}
					if (y - 1 > 0 && row[-stride + 3] != 0)
					{
						r += row[-stride];
						g += row[-stride + 1];
						b += row[-stride + 2];
						n++;
					}
					if (y + 1 < h && row[stride + 3] != 0)
					{
						r += row[stride];
						g += row[stride + 1];
						b += row[stride + 2];
						n++;
					}
					if (n > 0)
					{
						row[0] = static_cast<unsigned char>(r / n);
						row[1] = static_cast<unsigned char>(g / n);
						row[2] = static_cast<unsigned char>(b / n);
					}
				}
				row += 4;
			}
		}
	}

	static void FlatenShape(const SvgPath& path,
	                        std::vector<Edge>& edges,
	                        const double scale)
	{
		for (auto& pathInstance : path.GetInstances())
		{
			std::vector<RasterizerPoint> points;

			auto& curves = pathInstance.GetCurves();
			auto startPoint = pathInstance.GetStartPoint() * scale;
			// Flatten path
			AddPoint(points, startPoint, PointFlags::NONE);

			for (auto& curve : curves)
			{
					auto endPoint = curve.GetEnd() * scale;
				FlattenCubicBez(points,
				               startPoint,
				                curve.GetStartControl() * scale,
				                curve.GetEndControl() * scale,
				                endPoint,
				                0, PointFlags::NONE);
				startPoint = endPoint;
			}

			// Close path
			AddPoint(points, startPoint, PointFlags::NONE);

			// Build edges
			for (std::size_t i = 0, j = points.size() - 1; i < points.size(); j = i++)
			{
				const auto& a = points[j];
				const auto& b = points[i];
				AddEdge(edges, a.x, a.y, b.x, b.y);
			}
		}
	}

	static Paint GetColorPaint(const SvgPaint& paint, double opacity)
	{
		Paint returnPaint{};
		returnPaint.type = PAINT_COLOR;
		returnPaint.colors[0] = ApplyOpacity(paint.GetColor().GetValue(), opacity);
		return returnPaint;
	}

	static ActiveEdge CreateActive(const Edge& edge, const double startPoint)
	{
		ActiveEdge returnEdge{};

		const double dxdy = (edge.x1 - edge.x0) / (edge.y1 - edge.y0);
		//	STBTT_assert(e->y0 <= start_point);
		// round dx down to avoid going too far
		if (dxdy < 0)
		{
			returnEdge.dx = static_cast<int>(-RoundF(Fix * -dxdy));
		}
		else
		{
			returnEdge.dx = static_cast<int>(RoundF(Fix * dxdy));
		}

		returnEdge.x = static_cast<int>(RoundF(Fix * (edge.x0 + dxdy * (startPoint - edge.y0))));
		//	z->x -= off_x * FIX;
		returnEdge.ey = edge.y1;
		returnEdge.dir = edge.direction;

		return returnEdge;
	}

	static void FillScanline(
		unsigned char* scanline,
		const int len,
		const int x0,
		const int x1,
		const int maxWeight,
		int& xmin,
		int& xmax)
	{
		int i = x0 >> FixShift;
		int j = x1 >> FixShift;

		if (i < xmin) xmin = i;
		if (j > xmax) xmax = j;

		if (i < len && j >= 0)
		{
			if (i == j)
			{
				// x0,x1 are the same pixel, so compute combined coverage
				scanline[i] = static_cast<unsigned char>(scanline[i] + ((x1 - x0) * maxWeight >> FixShift));
			}
			else
			// add antialiasing for x0
			{
				if (i >= 0)
				{
					scanline[i] = static_cast<unsigned char>(scanline[i] + (((Fix - (x0 & FixMask)) * maxWeight) >>
						FixShift));
				}
				else
				{
					i = -1; // clip
				}


				// add antialiasing for x1
				if (j < len)
				{
					scanline[j] = static_cast<unsigned char>(scanline[j] + (((x1 & FixMask) * maxWeight) >> FixShift));
				}
				else
				{
					j = len; // clip
				}


				// fill pixels between x0 and x1
				for (++i; i < j; ++i)
				{
					scanline[i] = static_cast<unsigned char>(scanline[i] + maxWeight);
				}
			}
		}
	}

	// note: this routine clips fills that extend off the edges... ideally this
	// wouldn't happen, but it could happen if the truetype glyph bounding boxes
	// are wrong, or if the user supplies a too-small bitmap
	static void FillActiveEdges(unsigned char* scanline,
	                            const int len,
	                            const std::list<ActiveEdge>& list,
	                            std::list<ActiveEdge>::iterator iterator,
	                            const int maxWeight,
	                            int& xmin,
	                            int& xmax,
	                            const SvgFillRule fillRule)
	{
		// non-zero winding fill
		int x0 = 0, w = 0;

		if (fillRule == SvgFillRule::NonZero)
		{
			// Non-zero
			for (; iterator != list.end(); ++iterator)
			{
				if (w == 0)
				{
					// if we're currently at zero, we need to record the edge start point
					x0 = iterator->x;
					w += iterator->dir;
				}
				else
				{
					w += iterator->dir;
					// if we went to zero, we need to draw
					if (w == 0)
					{
						FillScanline(scanline, len, x0, iterator->x, maxWeight, xmin, xmax);
					}
				}
			}
		}
		else if (fillRule == SvgFillRule::EvenOdd)
		{
			// Even-odd
			for (; iterator != list.end(); ++iterator)
			{
				if (w == 0)
				{
					// if we're currently at zero, we need to record the edge start point
					x0 = iterator->x;
					w = 1;
				}
				else
				{
					w = 0;
					FillScanline(scanline, len, x0, iterator->x, maxWeight, xmin, xmax);
				}
			}
		}
	}

	static void ScanlineSolid(unsigned char* dst, std::size_t count, unsigned char* cover, int x, int y,
	                          double tx, double ty, double scale, const Paint& cache)
	{
		if (cache.type == PAINT_COLOR)
		{
			const int cr = cache.colors[0] & 0xff;
			const int cg = (cache.colors[0] >> 8) & 0xff;
			const int cb = (cache.colors[0] >> 16) & 0xff;
			const int ca = (cache.colors[0] >> 24) & 0xff;

			for (std::size_t i = 0; i < count; i++)
			{
				int a = Div255(static_cast<int>(cover[0]) * ca);
				const int ia = 255 - a;

				// Premultiply
				int r = Div255(cr * a);
				int g = Div255(cg * a);
				int b = Div255(cb * a);

				// Blend over
				r += Div255(ia * static_cast<int>(dst[0]));
				g += Div255(ia * static_cast<int>(dst[1]));
				b += Div255(ia * static_cast<int>(dst[2]));
				a += Div255(ia * static_cast<int>(dst[3]));

				dst[0] = static_cast<unsigned char>(r);
				dst[1] = static_cast<unsigned char>(g);
				dst[2] = static_cast<unsigned char>(b);
				dst[3] = static_cast<unsigned char>(a);

				cover++;
				dst += 4;
			}
		}
		else if (cache.type == PAINT_LINEAR_GRADIENT)
		{
			// TODO: spread modes.
			// TODO: plenty of opportunities to optimize.
			auto& t = cache.transform;

			double fx = (static_cast<double>(x) - tx) / scale;
			const double fy = (static_cast<double>(y) - ty) / scale;
			const double dx = 1.0 / scale;

			for (std::size_t i = 0; i < count; i++)
			{
				const double gy = fx * t[1] + fy * t[3] + t[5];
				const auto color = cache.colors[static_cast<int>(Clamp(gy * 255.0, 0, 255.0))];
				const auto cr = (color) & 0xff;
				const auto cg = (color >> 8) & 0xff;
				const auto cb = (color >> 16) & 0xff;
				const auto ca = (color >> 24) & 0xff;

				auto a = Div255(static_cast<int>(cover[0]) * ca);
				const auto ia = 255 - a;

				// Premultiply
				auto r = Div255(cr * a);
				auto g = Div255(cg * a);
				auto b = Div255(cb * a);

				// Blend over
				r += Div255(ia * static_cast<int>(dst[0]));
				g += Div255(ia * static_cast<int>(dst[1]));
				b += Div255(ia * static_cast<int>(dst[2]));
				a += Div255(ia * static_cast<int>(dst[3]));

				dst[0] = static_cast<unsigned char>(r);
				dst[1] = static_cast<unsigned char>(g);
				dst[2] = static_cast<unsigned char>(b);
				dst[3] = static_cast<unsigned char>(a);

				cover++;
				dst += 4;
				fx += dx;
			}
		}
		else if (cache.type == PAINT_RADIAL_GRADIENT)
		{
			// TODO: spread modes.
			// TODO: plenty of opportunities to optimize.
			// TODO: focus (fx,fy)
			auto& t = cache.transform;

			auto fx = (static_cast<double>(x) - tx) / scale;
			const auto fy = (static_cast<double>(y) - ty) / scale;
			const auto dx = 1.0 / scale;

			for (std::size_t i = 0; i < count; i++)
			{
				const auto gx = fx * t[0] + fy * t[2] + t[4];
				const auto gy = fx * t[1] + fy * t[3] + t[5];
				const auto gd = sqrt(gx * gx + gy * gy);
				const auto color = cache.colors[static_cast<int>(Clamp(gd * 255.0, 0, 255.0))];
				const auto cr = (color) & 0xff;
				const auto cg = (color >> 8) & 0xff;
				const auto cb = (color >> 16) & 0xff;
				const auto ca = (color >> 24) & 0xff;

				auto a = Div255(static_cast<int>(cover[0]) * ca);
				const auto ia = 255 - a;

				// Premultiply
				auto r = Div255(cr * a);
				auto g = Div255(cg * a);
				auto b = Div255(cb * a);

				// Blend over
				r += Div255(ia * static_cast<int>(dst[0]));
				g += Div255(ia * static_cast<int>(dst[1]));
				b += Div255(ia * static_cast<int>(dst[2]));
				a += Div255(ia * static_cast<int>(dst[3]));

				dst[0] = static_cast<unsigned char>(r);
				dst[1] = static_cast<unsigned char>(g);
				dst[2] = static_cast<unsigned char>(b);
				dst[3] = static_cast<unsigned char>(a);

				cover++;
				dst += 4;
				fx += dx;
			}
		}
	}


	static void RasterizeSortedEdges(unsigned char* scanline,
	                                 unsigned char* bitmap,
	                                 const std::size_t width,
	                                 const std::size_t stride,
	                                 const std::size_t height,
	                                 const double tx,
	                                 const double ty,
	                                 const double scale,
	                                 const std::vector<Edge>& edges,
	                                 const Paint& paint,
	                                 const SvgFillRule fillRule,
	                                 const std::size_t subSamples)
	{
		std::size_t edgeIndex = 0;
		const std::size_t maxWeight = 255 / subSamples; // weight per vertical scanline

		std::list<ActiveEdge> activeEdges;
		auto active = activeEdges.end();

		for (std::size_t y = 0; y < height; y++)
		{
			std::memset(scanline, 0, width);
			int xMin = width;
			int xMax = 0;
			for (std::size_t sample = 0; sample < subSamples; ++sample)
			{
				// find center of pixel for this scanline
				const auto scanY = static_cast<double>(y * subSamples + sample) + 0.5;

				// update all active edges;
				// remove all active edges that terminate before the center of this scanline
				for (auto step = active; step != activeEdges.end();)
				{
					if (step->ey <= scanY)
					{
						const bool updateActive = step == active;
						step = activeEdges.erase(step); // delete from list
						if (updateActive)
						{
							active = step;
						}
					}
					else
					{
						step->x += step->dx; // advance to position for current scanline
						++step; // advance through list
					}
				}

				while (true)
				{
					bool changed = false;

					for (auto step = active; step != activeEdges.end(); ++step)
					{
						auto next = step;
						if (++next == activeEdges.end()) break;

						if (step->x > next->x)
						{
							const bool updateActive = step == active;
							auto nextEdge = *next;
							activeEdges.erase(next);
							step = activeEdges.insert(step, nextEdge);
							if (updateActive)
							{
								active = step;
							}
							changed = true;
						}
					}

					if (!changed) break;
				}

				// insert all edges that start before the center of this scanline -- omit ones that also end on this scanline
				while (edgeIndex < edges.size() && edges[edgeIndex].y0 <= scanY)
				{
					if (edges[edgeIndex].y1 > scanY)
					{
						auto activeEdge = CreateActive(edges[edgeIndex], scanY);

						// find insertion point
						if (active == activeEdges.end())
						{
							active = activeEdges.insert(activeEdges.begin(), activeEdge);
						}
						else if (activeEdge.x < active->x)
						{
							// insert at front
							active = activeEdges.insert(active, activeEdge);
						}
						else
						{
							// find thing to insert AFTER
							auto itr = active;
							for (; itr != activeEdges.end(); ++itr)
							{
								if (itr->x >= activeEdge.x)
								{
									break;
								}
							}
							activeEdges.insert(itr, activeEdge);
						}
					}
					edgeIndex++;
				}

				// now process all active edges in non-zero fashion
				if (active != activeEdges.end())
				{
					FillActiveEdges(scanline, width, activeEdges, active, maxWeight, xMin, xMax, fillRule);
				}
			}

			//Blit
			if (xMin < 0) xMin = 0;
			if (xMax > width - 1) xMax = width - 1;
			if (xMin <= xMax)
			{
				ScanlineSolid(&bitmap[y * stride] + xMin * 4, xMax - xMin + 1, &scanline[xMin], xMin, y, tx, ty, scale,
				              paint);
			}
		}
	}

	static double Normalize(double& x, double& y)
	{
		const double d = sqrt(x * x + y * y);
		if (d > 1e-6)
		{
			const double id = 1.0 / d;
			x *= id;
			y *= id;
		}
		return d;
	}

	static void PrepareStroke(std::vector<RasterizerPoint>& points, const double miterLimit, const SvgLineJoin lineJoin)
	{
		// TODO: index based
		auto p0 = --points.end();
		auto p1 = points.begin();

		for (std::size_t i = 0; i < points.size(); ++i)
		{
			// Calculate segment direction and length
			p0->dx = p1->x - p0->x;
			p0->dy = p1->y - p0->y;
			p0->len = Normalize(p0->dx, p0->dy);

			// Advance
			p0 = p1++;
		}

		// calculate joins
		p0 = --points.end();
		p1 = points.begin();

		for (std::size_t i = 0; i < points.size(); ++i)
		{
			const double dlx0 = p0->dy;
			const double dly0 = -p0->dx;
			const double dlx1 = p1->dy;
			const double dly1 = -p1->dx;

			// Calculate extrusions
			p1->dmx = (dlx0 + dlx1) * 0.5;
			p1->dmy = (dly0 + dly1) * 0.5;

			const double dmr2 = p1->dmx * p1->dmx + p1->dmy * p1->dmy;
			if (dmr2 > 0.000001)
			{
				double s2 = 1.0 / dmr2;
				if (s2 > 600.0)
				{
					s2 = 600.0;
				}
				p1->dmx *= s2;
				p1->dmy *= s2;
			}

			// Clear flags, but keep the corner.
			p1->flags = static_cast<PointFlags>(p1->flags & PointFlags::CORNER ? PointFlags::CORNER : 0);

			// Keep track of left turns.
			const auto cross = p1->dx * p0->dy - p0->dx * p1->dy;
			if (cross > 0.0)
			{
				p1->flags = static_cast<PointFlags>(p1->flags | PointFlags::LEFT);
			}

			// Check to see if the corner needs to be beveled.
			if (p1->flags & PointFlags::CORNER)
			{
				if ((dmr2 * miterLimit * miterLimit) < 1.0
					|| lineJoin == SvgLineJoin::Bevel
					|| lineJoin == SvgLineJoin::Round)
				{
					p1->flags = static_cast<PointFlags>(p1->flags | PointFlags::BEVEL);
				}
			}

			p0 = p1++;
		}
	}

	static int CurveDivs(const double r, const double arc)
	{
		const double da = acos(r / (r + tessTol)) * 2.0;
		int divs = static_cast<int>(ceil(arc / da));
		if (divs < 2) divs = 2;
		return divs;
	}

	static void InitClosed(RasterizerPoint& left,
	                       RasterizerPoint& right,
	                       const RasterizerPoint& p0,
	                       const RasterizerPoint& p1,
	                       const double lineWidth)
	{
		const double w = lineWidth * 0.5;
		double dx = p1.x - p0.x;
		double dy = p1.y - p0.y;
		const double len = Normalize(dx, dy);
		const double px = p0.x + dx * len * 0.5;
		const double py = p0.y + dy * len * 0.5;
		const double dlx = dy;
		const double dly = -dx;
		const double lx = px - dlx * w;
		const double ly = py - dly * w;
		const double rx = px + dlx * w;
		const double ry = py + dly * w;
		left.x = lx;
		left.y = ly;
		right.x = rx;
		right.y = ry;
	}

	static void ButtCap(std::vector<Edge>& edges,
	                    RasterizerPoint& left,
	                    RasterizerPoint& right,
	                    RasterizerPoint& p,
	                    const double dx,
	                    const double dy,
	                    const double lineWidth,
	                    const bool connect)
	{
		const double w = lineWidth * 0.5;
		const double px = p.x;
		const double py = p.y;
		const double dlx = dy;
		const double dly = -dx;
		const double lx = px - dlx * w;
		const double ly = py - dly * w;
		const double rx = px + dlx * w;
		const double ry = py + dly * w;

		AddEdge(edges, lx, ly, rx, ry);

		if (connect)
		{
			AddEdge(edges, left.x, left.y, lx, ly);
			AddEdge(edges, rx, ry, right.x, right.y);
		}
		left.x = lx;
		left.y = ly;
		right.x = rx;
		right.y = ry;
	}

	static void SquareCap(std::vector<Edge>& edges,
	                      RasterizerPoint& left,
	                      RasterizerPoint& right,
	                      RasterizerPoint& p,
	                      const double dx,
	                      const double dy,
	                      const double lineWidth,
	                      const bool connect)
	{
		const double w = lineWidth * 0.5f;
		const double px = p.x - dx * w;
		const double py = p.y - dy * w;
		const double dlx = dy;
		const double dly = -dx;
		const double lx = px - dlx * w;
		const double ly = py - dly * w;
		const double rx = px + dlx * w;
		const double ry = py + dly * w;

		AddEdge(edges, lx, ly, rx, ry);

		if (connect)
		{
			AddEdge(edges, left.x, left.y, lx, ly);
			AddEdge(edges, rx, ry, right.x, right.y);
		}
		left.x = lx;
		left.y = ly;
		right.x = rx;
		right.y = ry;
	}

	static void RoundCap(std::vector<Edge>& edges,
	                     RasterizerPoint& left,
	                     RasterizerPoint& right,
	                     RasterizerPoint& p,
	                     const double dx,
	                     const double dy,
	                     const double lineWidth,
	                     const std::size_t ncap,
	                     const bool connect)
	{
		const double w = lineWidth * 0.5f;
		const double px = p.x;
		const double py = p.y;
		const double dlx = dy;
		const double dly = -dx;
		double lx = 0, ly = 0, rx = 0, ry = 0, prevx = 0, prevy = 0;

		for (std::size_t i = 0; i < ncap; i++)
		{
			const double a = static_cast<double>(i) / static_cast<double>(ncap - 1) * std::numbers::pi;
			const double ax = cos(a) * w;
			const double ay = sin(a) * w;
			const double x = px - dlx * ax - dx * ay;
			const double y = py - dly * ax - dy * ay;

			if (i > 0)
			{
				AddEdge(edges, prevx, prevy, x, y);
			}

			prevx = x;
			prevy = y;

			if (i == 0)
			{
				lx = x;
				ly = y;
			}
			else if (i == ncap - 1)
			{
				rx = x;
				ry = y;
			}
		}

		if (connect)
		{
			AddEdge(edges, left.x, left.y, lx, ly);
			AddEdge(edges, rx, ry, right.x, right.y);
		}

		left.x = lx;
		left.y = ly;
		right.x = rx;
		right.y = ry;
	}

	static void BevelJoin(std::vector<Edge>& edges,
	                      RasterizerPoint& left,
	                      RasterizerPoint& right,
	                      const RasterizerPoint& p0,
	                      const RasterizerPoint& p1,
	                      const double lineWidth)
	{
		const double w = lineWidth * 0.5;
		const double dlx0 = p0.dy;
		const double dly0 = -p0.dx;
		const double dlx1 = p1.dy;
		const double dly1 = -p1.dx;
		const double lx0 = p1.x - (dlx0 * w);
		const double ly0 = p1.y - (dly0 * w);
		const double rx0 = p1.x + (dlx0 * w);
		const double ry0 = p1.y + (dly0 * w);
		const double lx1 = p1.x - (dlx1 * w);
		const double ly1 = p1.y - (dly1 * w);
		const double rx1 = p1.x + (dlx1 * w);
		const double ry1 = p1.y + (dly1 * w);

		AddEdge(edges, lx0, ly0, left.x, left.y);
		AddEdge(edges, lx1, ly1, lx0, ly0);

		AddEdge(edges, right.x, right.y, rx0, ry0);
		AddEdge(edges, rx0, ry0, rx1, ry1);

		left.x = lx1;
		left.y = ly1;
		right.x = rx1;
		right.y = ry1;
	}

	static void MiterJoin(std::vector<Edge>& edges,
	                      RasterizerPoint& left,
	                      RasterizerPoint& right,
	                      const RasterizerPoint& p0,
	                      const RasterizerPoint& p1,
	                      const double lineWidth)
	{
		const double w = lineWidth * 0.5f;
		const double dlx0 = p0.dy;
		const double dly0 = -p0.dx;
		const double dlx1 = p1.dy;
		const double dly1 = -p1.dx;
		double lx0, rx0, lx1, rx1;
		double ly0, ry0, ly1, ry1;

		if (p1.flags & PointFlags::LEFT)
		{
			lx0 = lx1 = p1.x - p1.dmx * w;
			ly0 = ly1 = p1.y - p1.dmy * w;
			AddEdge(edges, lx1, ly1, left.x, left.y);

			rx0 = p1.x + (dlx0 * w);
			ry0 = p1.y + (dly0 * w);
			rx1 = p1.x + (dlx1 * w);
			ry1 = p1.y + (dly1 * w);
			AddEdge(edges, right.x, right.y, rx0, ry0);
			AddEdge(edges, rx0, ry0, rx1, ry1);
		}
		else
		{
			lx0 = p1.x - (dlx0 * w);
			ly0 = p1.y - (dly0 * w);
			lx1 = p1.x - (dlx1 * w);
			ly1 = p1.y - (dly1 * w);
			AddEdge(edges, lx0, ly0, left.x, left.y);
			AddEdge(edges, lx1, ly1, lx0, ly0);

			rx0 = rx1 = p1.x + p1.dmx * w;
			ry0 = ry1 = p1.y + p1.dmy * w;
			AddEdge(edges, right.x, right.y, rx1, ry1);
		}

		left.x = lx1;
		left.y = ly1;
		right.x = rx1;
		right.y = ry1;
	}

	static void RoundJoin(std::vector<Edge>& edges,
	                      RasterizerPoint& left,
	                      RasterizerPoint& right,
	                      const RasterizerPoint& p0,
	                      const RasterizerPoint& p1,
	                      const double lineWidth,
	                      const std::size_t ncap)
	{
		const double w = lineWidth * 0.5;
		const double dlx0 = p0.dy;
		const double dly0 = -p0.dx;
		const double dlx1 = p1.dy;
		const double dly1 = -p1.dx;
		const double a0 = atan2(dly0, dlx0);
		const double a1 = atan2(dly1, dlx1);
		double da = a1 - a0;

		if (da < std::numbers::pi) da += std::numbers::pi * 2;
		if (da > std::numbers::pi) da -= std::numbers::pi * 2;

		auto nV = static_cast<int>(ceil((AbsF(da) / std::numbers::pi) * static_cast<double>(ncap)));
		if (nV < 2) nV = 2;
		std::size_t n = nV;
		if (n > ncap) n = ncap;

		double lx = left.x;
		double ly = left.y;
		double rx = right.x;
		double ry = right.y;

		for (std::size_t i = 0; i < n; i++)
		{
			const double u = static_cast<double>(i) / static_cast<double>(n - 1);
			const double a = a0 + u * da;
			const double ax = cos(a) * w;
			const double ay = sin(a) * w;
			const double lx1 = p1.x - ax;
			const double ly1 = p1.y - ay;
			const double rx1 = p1.x + ax;
			const double ry1 = p1.y + ay;

			AddEdge(edges, lx1, ly1, lx, ly);
			AddEdge(edges, rx, ry, rx1, ry1);

			lx = lx1;
			ly = ly1;
			rx = rx1;
			ry = ry1;
		}

		left.x = lx;
		left.y = ly;
		right.x = rx;
		right.y = ry;
	}

	static void StraightJoin(std::vector<Edge>& edges,
	                         RasterizerPoint& left,
	                         RasterizerPoint& right,
	                         const RasterizerPoint& p1,
	                         const double lineWidth)
	{
		const double w = lineWidth * 0.5;
		const double lx = p1.x - (p1.dmx * w);
		const double ly = p1.y - (p1.dmy * w);
		const double rx = p1.x + (p1.dmx * w);
		const double ry = p1.y + (p1.dmy * w);

		AddEdge(edges, lx, ly, left.x, left.y);
		AddEdge(edges, right.x, right.y, rx, ry);

		left.x = lx;
		left.y = ly;
		right.x = rx;
		right.y = ry;
	}

	static void ExpandStroke(std::vector<Edge>& edges, std::vector<RasterizerPoint>& points, bool closed,
	                         const SvgLineJoin lineJoin, const SvgLineCap lineCap, double lineWidth)
	{
		const auto ncap = CurveDivs(lineWidth * 0.5, std::numbers::pi);
		RasterizerPoint left{}, right{}, firstLeft{}, firstRight{};

		std::vector<RasterizerPoint>::iterator p0;
		std::vector<RasterizerPoint>::iterator p1;

		std::size_t s, e;

		// Build stroke edges
		if (closed)
		{
			p0 = --points.end();
			p1 = points.begin();
			s = 0;
			e = points.size();
		}
		else
		{
			p0 = points.begin();
			p1 = ++points.begin();
			s = 1;
			e = points.size() - 1;
		}

		if (closed)
		{
			InitClosed(left, right, *p0, *p1, lineWidth);
			firstLeft = left;
			firstRight = right;
		}
		else
		{
			// Add cap
			double dx = p1->x - p0->x;
			double dy = p1->y - p0->y;
			Normalize(dx, dy);
			switch (lineCap)
			{
			case SvgLineCap::Butt:
				ButtCap(edges, left, right, *p0, dx, dy, lineWidth, false);
				break;
			case SvgLineCap::Round:
				RoundCap(edges, left, right, *p0, dx, dy, lineWidth, ncap, false);
				break;
			case SvgLineCap::Square:
				SquareCap(edges, left, right, *p0, dx, dy, lineWidth, false);
				break;
			}
		}

		for (std::size_t i = s; i < e; ++i)
		{
			if (p1->flags & PointFlags::CORNER)
			{
				if (lineJoin == SvgLineJoin::Round)
				{
					RoundJoin(edges, left, right, *p0, *p1, lineWidth, ncap);
				}
				else if (lineJoin == SvgLineJoin::Bevel || (p1->flags & PointFlags::BEVEL))
				{
					BevelJoin(edges, left, right, *p0, *p1, lineWidth);
				}
				else
				{
					MiterJoin(edges, left, right, *p0, *p1, lineWidth);
				}
			}
			else
			{
				StraightJoin(edges, left, right, *p1, lineWidth);
			}
			p0 = p1++;
		}

		if (closed)
		{
			// Loop it
			AddEdge(edges, firstLeft.x, firstLeft.y, left.x, left.y);
			AddEdge(edges, right.x, right.y, firstRight.x, firstRight.y);
		}
		else
		{
			// Add cap
			double dx = p1->x - p0->x;
			double dy = p1->y - p0->y;
			Normalize(dx, dy);
			if (lineCap == SvgLineCap::Butt)
			{
				ButtCap(edges, right, left, *p1, -dx, -dy, lineWidth, true);
			}
			else if (lineCap == SvgLineCap::Square)
			{
				SquareCap(edges, right, left, *p1, -dx, -dy, lineWidth, true);
			}
			else if (lineCap == SvgLineCap::Round)
			{
				RoundCap(edges, right, left, *p1, -dx, -dy, lineWidth, ncap, true);
			}
		}
	}

	static void FlattenShapeStroke(const SvgStroke& stroke,
	                               const std::vector<SvgPathInstance>& paths,
	                               std::vector<Edge>& edges,
	                               const double scale)
	{
		const double lineWidth = stroke.GetWidth() * scale;
		for (auto& path : paths)
		{
			auto& curves = path.GetCurves();
			if (curves.empty()) continue;

			std::vector<RasterizerPoint> points;

			auto startPoint = path.GetStartPoint() * scale;
			AddPoint(points, startPoint, PointFlags::CORNER);

			for (const auto& curve : curves)
			{
				auto endPoint = curve.GetEnd() * scale;
				FlattenCubicBez(points,
				                startPoint,
				                curve.GetStartControl() * scale,
				                curve.GetEndControl() * scale,
				                endPoint,
				                0, PointFlags::CORNER);

				startPoint = endPoint;
			}

			if (points.size() < 2)
			{
				continue;
			}

			bool closed = path.IsClosed();

			// If the first and last points are the same, remove the last, mark as closed path.
			if (PointEquals(points.front(), points.back()))
			{
				points.pop_back();
				closed = true;
			}

			auto& dashesArray = stroke.GetDashes();
			if (!dashesArray.empty())
			{
				bool dashState = false;
				if (closed)
				{
					points.push_back(points.front());
				}

				// Duplicate points -> points2.
				auto points2 = points;

				points.clear();

				auto* current = &points2.front();
				points.push_back(*current);

				// Figure out dash offset.
				double allDashLength = 0.0;
				for (const auto dash : dashesArray)
				{
					allDashLength += dash;
				}

				if (dashesArray.size() & 1)
				{
					allDashLength *= 2.0;
				}

				// Find location inside pattern
				auto dashOffset = fmod(stroke.GetDashOffset(), allDashLength);
				if (dashOffset < 0.0)
				{
					dashOffset += allDashLength;
				}

				std::size_t dashIndex = 0;
				while (dashOffset > dashesArray[dashIndex])
				{
					dashOffset -= dashesArray[dashIndex];
					dashIndex = (dashIndex + 1) % dashesArray.size();
				}

				auto dashLength = (dashesArray[dashIndex] - dashOffset) * scale;
				double totalDistance = 0.0;
				for (std::size_t j = 1; j < points2.size();)
				{
					const double dx = points2[j].x - current->x;
					const double dy = points2[j].y - current->y;
					const double distance = sqrt((dx * dx) + (dy * dy));

					if (totalDistance + distance > dashLength)
					{
						// Calculate intermediate point
						const double d = (dashLength - totalDistance) / distance;
						const double x = current->x + dx * d;
						const double y = current->y + dy * d;

						AddPoint(points, SvgPoint(x, y), PointFlags::CORNER);

						// Stroke
						if (points.size() > 1 && dashState)
						{
							PrepareStroke(points, stroke.GetMiterLimit(), stroke.GetLineJoin());
							ExpandStroke(edges, points, false, stroke.GetLineJoin(), stroke.GetLineCap(), lineWidth);
						}

						// Advance dash pattern
						dashState = !dashState;
						dashIndex = (dashIndex + 1) % dashesArray.size();
						dashLength = dashesArray[dashIndex] * scale;

						// Restart
						current->x = x;
						current->y = y;
						current->flags = PointFlags::CORNER;
						totalDistance = 0.0;
						points.clear();
						points.push_back(*current);
					}
					else
					{
						totalDistance += distance;
						current = &points2[j];
						points.push_back(*current);
						j++;
					}
				}
				if (points.size() > 1 && dashState)
				{
					ExpandStroke(edges, points, false, stroke.GetLineJoin(), stroke.GetLineCap(), lineWidth);
				}
			}
			else
			{
				PrepareStroke(points, stroke.GetMiterLimit(), stroke.GetLineJoin());
				ExpandStroke(edges, points, closed, stroke.GetLineJoin(), stroke.GetLineCap(), lineWidth);
			}
		}
	}

	static SvgBounds GetLocalBounds(const SvgPath& shape, const SvgTransform& transform)
	{
		SvgBounds returnBounds;

		bool first = true;
		for (auto& path : shape.GetInstances())
		{
			SvgPoint curve[4];

			auto& a = curve[0];
			auto& curves = path.GetCurves();
			auto startPoint = path.GetStartPoint();

			transform.ApplyToPoint(a.GetRefX(), a.GetRefY(), startPoint.GetX(), startPoint.GetY());

			for (const auto& currentCurve : curves)
			{
				auto& b = curve[1];
				auto& c = curve[2];
				auto& d = curve[3];

				auto& pB = currentCurve.GetStartControl();
				auto& pC = currentCurve.GetEndControl();
				auto& pD = currentCurve.GetEnd();

				transform.ApplyToPoint(b.GetRefX(), b.GetRefY(), pB.GetX(), pB.GetY());
				transform.ApplyToPoint(c.GetRefX(), c.GetRefY(), pC.GetX(), pC.GetY());
				transform.ApplyToPoint(d.GetRefX(), d.GetRefY(), pD.GetX(), pD.GetY());

				if (first)
				{
					returnBounds = SvgBounds(a, b, c, d);
					first = false;
				}
				else
				{
					returnBounds.Merge(SvgBounds(a, b, c, d));
				}
				curve[0].GetRefX() = curve[3].GetX();
				curve[0].GetRefY() = curve[3].GetY();
			}
		}

		return returnBounds;
	}

	static Paint CalculateGradient(const SvgDocument& document, const SvgPath& shape, const SvgPaint& paint, double opacity)
	{
		Paint returnPaint{};

		const auto gradientId = paint.GetGradientId().substr(1); //ignore the '#'
		const auto gradientItr = document.GetDefs().find(gradientId);
		if (gradientItr != document.GetDefs().end())
		{
			const auto gradient = dynamic_cast<SvgGradient*>(gradientItr->second.get());
			if (gradient == nullptr) return GetColorPaint(paint, opacity);

			const auto radialGradient = dynamic_cast<SvgRadialGradient*>(gradient);
			const auto linear = dynamic_cast<SvgLinearGradient*>(gradient);
			returnPaint.type = radialGradient != nullptr ? PAINT_RADIAL_GRADIENT : PAINT_LINEAR_GRADIENT;
			returnPaint.transform = gradient->GetGradientTransform();

			auto gradientStops = gradientItr;
			if (gradient->GetStops().empty())
			{
				auto& ref = gradient->GetHref();
				if (!ref.empty())
				{
					gradientStops = document.GetDefs().find(ref.substr(1));
				}
			}

			auto& stops = dynamic_cast<SvgGradient*>(gradientStops->second.get())->GetStops();

			returnPaint.transform.Inverse(shape.GetTransform());
			const auto bounds = GetLocalBounds(shape, returnPaint.transform);

			double ox, oy, sw, sh;
			if (gradient->GetUnits() == SvgGradientUnits::Object)
			{
				ox = bounds.GetMinX();
				oy = bounds.GetMinY();
				sw = bounds.GetMaxX() - bounds.GetMinX();
				sh = bounds.GetMaxY()- bounds.GetMinY();
			}
			else
			{
				ox = document.GetElement().GetViewBox().GetMinX();
				oy = document.GetElement().GetViewBox().GetMinY();
				sw = document.GetElement().GetViewBox().GetWidth();
				sh = document.GetElement().GetViewBox().GetHeight();
			}

			const auto sl = sqrt(sw*sw + sh*sh) / sqrt(2.0);

			if (radialGradient == nullptr)
			{
				const double x1 = linear->GetX1().CalculateActualValue(document, ox, sw);
				const double y1 = linear->GetY1().CalculateActualValue(document, oy, sh);
				const double x2 = linear->GetX2().CalculateActualValue(document, ox, sw);
				const double y2 = linear->GetY2().CalculateActualValue(document, oy, sh);

				// Calculate transform aligned to the line
				const double dx = x2 - x1;
				const double dy = y2 - y1;

				returnPaint.transform = SvgTransform(dy, -dx, dx,dy,x1,y1);
			}
			else
			{
				const double cx = radialGradient->GetCx().CalculateActualValue(document, ox, sw);
				const double cy = radialGradient->GetCy().CalculateActualValue(document, oy, sh);
				double fx = radialGradient->GetFx().CalculateActualValue(document, ox, sw);
				double fy = radialGradient->GetFy().CalculateActualValue(document, oy, sh);
				const double r = radialGradient->GetR().CalculateActualValue(document, 0, sl);

				returnPaint.transform = SvgTransform(r, 0, 0,r,cx,cy);
			}

			if (stops.empty())
			{
				for (unsigned int& color : returnPaint.colors)
				{
					color = 0;
				}
			}
			else if (stops.size() == 1)
			{
				const auto cachedColor = ApplyOpacity(stops.front().GetColor().GetValue(), opacity);
				for (unsigned int& color : returnPaint.colors)
				{
					color = cachedColor;
				}
			}
			else
			{
				unsigned int cb = 0;

				auto& firstStop = stops.front();

				unsigned int ca = ApplyOpacity(firstStop.GetColor().GetValue(), opacity * firstStop.GetOpacity());

				double ua = Clamp(firstStop.GetOffset(), 0, 1);
				double ub = Clamp(stops.back().GetOffset(), ua, 1);

				auto ia = static_cast<std::size_t>(ua * 255.0);
				auto ib = static_cast<std::size_t>(ub * 255.0);

				for (std::size_t i = 0; i < ia; i++)
				{
					returnPaint.colors[i] = ca;
				}

				for (std::size_t i = 0; i < stops.size() - 1; i++)
				{
					auto& a = stops[i];
					auto& b = stops[i + 1];

					ca = ApplyOpacity(a.GetColor().GetValue(), opacity * a.GetOpacity());
					cb = ApplyOpacity(b.GetColor().GetValue(), opacity * b.GetOpacity());

					ua = Clamp(a.GetOffset(), 0, 1);
					ub = Clamp(b.GetOffset(), 0, 1);

					ia = static_cast<std::size_t>(ua * 255.0);
					ib = static_cast<std::size_t>(ub * 255.0);

					if (ib - ia <= 0) continue;
					const std::size_t count = ib - ia;

					const double du = 1.0 / static_cast<double>(count);
					double u = 0;
					for (std::size_t j = 0; j < static_cast<std::size_t>(count); j++)
					{
						returnPaint.colors[ia + j] = ShiftColorToColor(ca, cb, u);
						u += du;
					}
				}

				for (std::size_t i = ib; i < 256; i++)
				{
					returnPaint.colors[i] = cb;
				}
			}
		}
		else
		{
			return GetColorPaint(paint, opacity);
		}


		return returnPaint;
	}

	static Paint GetPaint( const SvgDocument& document, const SvgPath& shape, const SvgPaint& paint, double opacity)
	{
		Paint returnPaint{};
		if (paint.GetGradientId().empty())
		{
			return GetColorPaint(paint, opacity);
		}
		else
		{
			return CalculateGradient(document, shape, paint, opacity);

		}

		return returnPaint;
	}



	void SvgRasterizer::Rasterize(const SvgDocument& document,
	                              unsigned char* outputBuffer,
	                              std::size_t width,
	                              std::size_t height,
	                              std::size_t stride,
	                              double tx, double ty,
	                              double scale)
	{
		std::memset(outputBuffer, 0, width * 4);

		std::unique_ptr<unsigned char[]> scanLine(new unsigned char[width]);
		for (auto& element : document.GetElement().GetChildren())
		{
			if (const auto shape = dynamic_cast<SvgPath*>(element.get()))
			{
				if (!shape->IsVisible()) continue;
				if (shape->GetFill().IsSet())
				{
					std::vector<Edge> edges;

					FlatenShape(*shape, edges, scale);

					// Scale and translate edges
					for (auto& edge : edges)
					{
						edge.x0 = tx + edge.x0;
						edge.y0 = (ty + edge.y0) * SubSamples;
						edge.x1 = tx + edge.x1;
						edge.y1 = (ty + edge.y1) * SubSamples;
					}

					std::ranges::sort(edges, [](const Edge& a, const Edge& b)
					{
						return a.y0 < b.y0;
					});

					// now, traverse the scanlines and find the intersections on each scanline, use non-zero rule

					auto paint = GetPaint(document, *shape, shape->GetFill(), shape->GetOpacity());

					RasterizeSortedEdges(scanLine.get(), outputBuffer, width, stride, height, tx, ty, scale, edges,
					                     paint, shape->GetFill().GetFillRule(), SubSamples);
				}

				if (shape->GetStroke().IsSet())
				{
					std::vector<Edge> edges;

					FlattenShapeStroke(shape->GetStroke(), shape->GetInstances(), edges, scale);


					// Scale and translate edges
					for (auto& edge : edges)
					{
						edge.x0 = tx + edge.x0;
						edge.y0 = (ty + edge.y0) * SubSamples;
						edge.x1 = tx + edge.x1;
						edge.y1 = (ty + edge.y1) * SubSamples;
					}

					std::ranges::sort(edges, [](const Edge& a, const Edge& b)
					{
						return a.y0 < b.y0;
					});

					auto paint = GetPaint(document, *shape, shape->GetStroke(), shape->GetOpacity());

					RasterizeSortedEdges(scanLine.get(), outputBuffer, width, stride, height, tx, ty, scale, edges,
					                     paint, SvgFillRule::NonZero, SubSamples);
				}
			}
		}

		UnpremultiplyAlpha(outputBuffer, width, height, stride);
	}
} // Elpida
