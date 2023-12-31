//
// Created by klapeto on 24/12/2023.
//

#include "Elpida/Svg/SvgRasterizer.hpp"

#include <algorithm>
#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgGradient.hpp>
#include <Elpida/Svg/SvgPath.hpp>
#include <Elpida/Svg/SvgPaint.hpp>
#include <Elpida/Svg/SvgRadialGradient.hpp>
#include <Elpida/Svg/SvgShape.hpp>

#include <cstring>
#include <list>

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
		unsigned char flags;
	};

	struct ActiveEdge
	{
		int x, dx;
		double ey;
		int dir;

		bool operator < (const ActiveEdge& other) const
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

	static int Div255(const int x)
	{
		return ((x+1) * 257) >> 16;
	}

	static double Clamp(const double a, const double mn, const double mx)
	{
		return a < mn ? mn : (a > mx ? mx : a);
	}

	static unsigned int RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		return static_cast<unsigned int>(r)
			| (static_cast<unsigned int>(g) << 8)
			| (static_cast<unsigned int>(b) << 16)
			| (static_cast<unsigned int>(a) << 24);
	}

	static unsigned int LerpRGBA(unsigned int c0, unsigned int c1, double u)
	{
		int iu = (int)(Clamp(u, 0.0f, 1.0f) * 256.0f);
		int r = (((c0) & 0xff) * (256 - iu) + (((c1) & 0xff) * iu)) >> 8;
		int g = (((c0 >> 8) & 0xff) * (256 - iu) + (((c1 >> 8) & 0xff) * iu)) >> 8;
		int b = (((c0 >> 16) & 0xff) * (256 - iu) + (((c1 >> 16) & 0xff) * iu)) >> 8;
		int a = (((c0 >> 24) & 0xff) * (256 - iu) + (((c1 >> 24) & 0xff) * iu)) >> 8;
		return RGBA((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a);
	}

	static unsigned int ApplyOpacity(unsigned int c, double u)
	{
		const int iu = static_cast<int>(Clamp(u, 0.0, 1.0) * 256.0);
		const int r = (c) & 0xff;
		const int g = (c >> 8) & 0xff;
		const int b = (c >> 16) & 0xff;
		const int a = (((c >> 24) & 0xff) * iu) >> 8;
		return RGBA(
			static_cast<unsigned char>(r),
			static_cast<unsigned char>(g),
			static_cast<unsigned char>(b),
			static_cast<unsigned char>(a));
	}

	static void AddPoint(std::vector<RasterizerPoint>& points, const double x, const double y, const int flags)
	{
		if (!points.empty())
		{
			auto& lastPoint = points.back();
			if (PointEquals(lastPoint.x, lastPoint.y, x, y))
			{
				lastPoint.flags = static_cast<unsigned char>(lastPoint.flags | flags);
				return;
			}
		}

		points.push_back({
			.x = x,
			.y = y,
			.dx = 0,
			.dy = 0,
			.len = 0,
			.dmx = 0,
			.dmy = 0,
			.flags = 0
		});
	}

	static void FlattenCubicBez(std::vector<RasterizerPoint>& points,
	                            const double x1, const double y1,
	                            const double x2, const double y2,
	                            const double x3, const double y3,
	                            const double x4, const double y4,
	                            const int level, const int type)
	{
		if (level > 10) return;

		const double x12 = (x1 + x2) * 0.5;
		const double y12 = (y1 + y2) * 0.5;
		const double x23 = (x2 + x3) * 0.5;
		const double y23 = (y2 + y3) * 0.5;
		const double x34 = (x3 + x4) * 0.5;
		const double y34 = (y3 + y4) * 0.5;
		const double x123 = (x12 + x23) * 0.5;
		const double y123 = (y12 + y23) * 0.5;

		const double dx = x4 - x1;
		const double dy = y4 - y1;
		const double d2 = AbsF(((x2 - x4) * dy) - ((y2 - y4) * dx));
		const double d3 = AbsF(((x3 - x4) * dy) - ((y3 - y4) * dx));

		if ((d2 + d3) * (d2 + d3) < tessTol * (dx * dx + dy * dy))
		{
			AddPoint(points, x4, y4, type);
			return;
		}

		const double x234 = (x23 + x34) * 0.5;
		const double y234 = (y23 + y34) * 0.5;
		const double x1234 = (x123 + x234) * 0.5;
		const double y1234 = (y123 + y234) * 0.5;

		FlattenCubicBez(points, x1, y1, x12, y12, x123, y123, x1234, y1234, level + 1, 0);
		FlattenCubicBez(points, x1234, y1234, x234, y234, x34, y34, x4, y4, level + 1, type);
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

	static void UnpremultiplyAlpha(unsigned char* image, int w, int h, int stride)
	{
		int x,y;

		// Unpremultiply
		for (y = 0; y < h; y++) {
			unsigned char *row = &image[y*stride];
			for (x = 0; x < w; x++) {
				int r = row[0], g = row[1], b = row[2], a = row[3];
				if (a != 0) {
					row[0] = (unsigned char)(r*255/a);
					row[1] = (unsigned char)(g*255/a);
					row[2] = (unsigned char)(b*255/a);
				}
				row += 4;
			}
		}

		// Defringe
		for (y = 0; y < h; y++) {
			unsigned char *row = &image[y*stride];
			for (x = 0; x < w; x++) {
				int r = 0, g = 0, b = 0, a = row[3], n = 0;
				if (a == 0) {
					if (x-1 > 0 && row[-1] != 0) {
						r += row[-4];
						g += row[-3];
						b += row[-2];
						n++;
					}
					if (x+1 < w && row[7] != 0) {
						r += row[4];
						g += row[5];
						b += row[6];
						n++;
					}
					if (y-1 > 0 && row[-stride+3] != 0) {
						r += row[-stride];
						g += row[-stride+1];
						b += row[-stride+2];
						n++;
					}
					if (y+1 < h && row[stride+3] != 0) {
						r += row[stride];
						g += row[stride+1];
						b += row[stride+2];
						n++;
					}
					if (n > 0) {
						row[0] = (unsigned char)(r/n);
						row[1] = (unsigned char)(g/n);
						row[2] = (unsigned char)(b/n);
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
		std::vector<RasterizerPoint> points;
		for (auto& pathInstance : path.GetInstances())
		{
			points.clear();
			auto& pathPoints = pathInstance.GetPoints();
			auto& firstPoint = pathPoints.front();

			// Flatten path
			AddPoint(points, firstPoint.GetX() * scale, firstPoint.GetY() * scale, 0);

			for (std::size_t i = 0; i < pathPoints.size() - 1; i += 3)
			{
				auto& a = pathPoints[i];
				auto& b = pathPoints[i + 1];
				auto& c = pathPoints[i + 2];
				auto& d = pathPoints[i + 3];

				FlattenCubicBez(points,
				                a.GetX() * scale, a.GetY() * scale,
				                b.GetX() * scale, b.GetY() * scale,
				                c.GetX() * scale, c.GetY() * scale,
				                d.GetX() * scale, d.GetY() * scale,
				                0, 0);
			}

			// Close path
			AddPoint(points, firstPoint.GetX() * scale, firstPoint.GetY() * scale, 0);

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

	static Paint GetPaint(const SvgDefs& defs, const SvgPaint& paint, double opacity)
	{
		Paint returnPaint{};
		if (paint.GetGradientId().empty())
		{
			return GetColorPaint(paint, opacity);
		}
		else
		{
			auto gradientItr = defs.find(paint.GetGradientId());
			if (gradientItr != defs.end())
			{
				auto gradient = dynamic_cast<SvgGradient*>(gradientItr->second.get());
				if (gradient == nullptr) return GetColorPaint(paint, opacity);

				auto radialGradient = dynamic_cast<SvgRadialGradient*>(gradient);
				returnPaint.type = radialGradient != nullptr ? PAINT_RADIAL_GRADIENT : PAINT_LINEAR_GRADIENT;
				returnPaint.transform = gradient->GetGradientTransform();

				auto& stops = gradient->GetStops();
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
					unsigned int ca = ApplyOpacity(firstStop.GetColor().GetValue(), opacity);

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

						ca = ApplyOpacity(a.GetColor().GetValue(), opacity);
						cb = ApplyOpacity(b.GetColor().GetValue(), opacity);

						ua = Clamp(a.GetOffset(), 0, 1);
						ub = Clamp(b.GetOffset(), 0, 1);

						ia = static_cast<std::size_t>(ua * 255.0);
						ib = static_cast<int>(ub * 255.0);

						if (ib - ia <= 0) continue;
						const std::size_t count = ib-ia;

						const double du = 1.0f / static_cast<double>(count);
						double u = 0;
						for (std::size_t j = 0; j < static_cast<std::size_t>(count); j++)
						{
							returnPaint.colors[ia + j] = LerpRGBA(ca, cb, u);
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
		}

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

	static void FillScanline(unsigned char* scanline, const int len, const int x0, const int x1, const int maxWeight, int& xmin, int& xmax)
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
					scanline[i] = static_cast<unsigned char>(scanline[i] + (((Fix - (x0 & FixMask)) * maxWeight) >> FixShift));
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
		} else
		{
			int x= 5;
		}
	}

	// note: this routine clips fills that extend off the edges... ideally this
	// wouldn't happen, but it could happen if the truetype glyph bounding boxes
	// are wrong, or if the user supplies a too-small bitmap
	static void FillActiveEdges(unsigned char* scanline, int len, const std::list<ActiveEdge>& list,
	                            std::list<ActiveEdge>::iterator iterator, int maxWeight, int& xmin, int& xmax,
	                            SvgFillRule fillRule)
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
					const int x1 = iterator->x;
					w += iterator->dir;
					// if we went to zero, we need to draw
					if (w == 0)
					{
						FillScanline(scanline, len, x0, x1, maxWeight, xmin, xmax);
					}
				}
			}
		}
		else if (fillRule == SvgFillRule::EvenOdd)
		{
			// Even-odd
			while (iterator != list.end())
			{
				if (w == 0)
				{
					// if we're currently at zero, we need to record the edge start point
					x0 = iterator->x;
					w = 1;
				}
				else
				{
					const int x1 = iterator->x;
					w = 0;
					FillScanline(scanline, len, x0, x1, maxWeight, xmin, xmax);
				}
				iterator = ++iterator;
			}
		}
	}

	static void ScanlineSolid(unsigned char* dst, std::size_t count, unsigned char* cover, int x, int y,
	                          double tx, double ty, double scale, const Paint& cache)
	{
		if (cache.type == PAINT_COLOR)
		{
			int cr, cg, cb, ca;
			cr = cache.colors[0] & 0xff;
			cg = (cache.colors[0] >> 8) & 0xff;
			cb = (cache.colors[0] >> 16) & 0xff;
			ca = (cache.colors[0] >> 24) & 0xff;

			for (std::size_t i = 0; i < count; i++)
			{
				int r, g, b;
				int a = Div255(static_cast<int>(cover[0]) * ca);
				int ia = 255 - a;

				// Premultiply
				r = Div255(cr * a);
				g = Div255(cg * a);
				b = Div255(cb * a);

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
			double fx, fy, dx, gy;
			auto& t = cache.transform;
			int cr, cg, cb, ca;
			unsigned int c;

			fx = (static_cast<double>(x) - tx) / scale;
			fy = (static_cast<double>(y) - ty) / scale;
			dx = 1.0f / scale;

			for (std::size_t i = 0; i < count; i++)
			{
				int r, g, b, a, ia;
				gy = fx * t[1] + fy * t[3] + t[5];
				c = cache.colors[static_cast<int>(Clamp(gy * 255.0, 0, 255.0))];
				cr = (c) & 0xff;
				cg = (c >> 8) & 0xff;
				cb = (c >> 16) & 0xff;
				ca = (c >> 24) & 0xff;

				a = Div255(static_cast<int>(cover[0]) * ca);
				ia = 255 - a;

				// Premultiply
				r = Div255(cr * a);
				g = Div255(cg * a);
				b = Div255(cb * a);

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
			double fx, fy, dx, gx, gy, gd;
			auto& t = cache.transform;
			int  cr, cg, cb, ca;
			unsigned int c;

			fx = (static_cast<double>(x) - tx) / scale;
			fy = (static_cast<double>(y) - ty) / scale;
			dx = 1.0f / scale;

			for (std::size_t i = 0; i < count; i++)
			{
				int r, g, b, a, ia;
				gx = fx * t[0] + fy * t[2] + t[4];
				gy = fx * t[1] + fy * t[3] + t[5];
				gd = sqrt(gx * gx + gy * gy);
				c = cache.colors[static_cast<int>(Clamp(gd * 255.0f, 0, 255.0f))];
				cr = (c) & 0xff;
				cg = (c >> 8) & 0xff;
				cb = (c >> 16) & 0xff;
				ca = (c >> 24) & 0xff;

				a = Div255(static_cast<int>(cover[0]) * ca);
				ia = 255 - a;

				// Premultiply
				r = Div255(cr * a);
				g = Div255(cg * a);
				b = Div255(cb * a);

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
		const std::size_t maxWeight = 255 / subSamples;  // weight per vertical scanline

		std::list<ActiveEdge> activeEdges;
		std::list<ActiveEdge>::iterator active;

		for (std::size_t y = 0; y < height; y++)
		{
			std::memset(scanline, 0, width);
			int xMin = width;
			int xMax = 0;
			for (std::size_t sample = 0; sample < subSamples; ++sample)
			{
				// find center of pixel for this scanline
				const auto scanY = static_cast<double>(y * subSamples + sample) + 0.5;
				auto step = active;

				// update all active edges;
				// remove all active edges that terminate before the center of this scanline

				while (step != std::list<ActiveEdge>::iterator() && step != activeEdges.end())
				{
					if (step->ey <= scanY)
					{
						step = activeEdges.erase(step); // delete from list
						active = step;
					}
					else
					{
						step->x += step->dx; // advance to position for current scanline
						++step;
					}
				}
				activeEdges.sort();
				// insert all edges that start before the center of this scanline -- omit ones that also end on this scanline
				while (edgeIndex < edges.size() && edges[edgeIndex].y0 <= scanY)
				{
					if (edges[edgeIndex].y1 > scanY)
					{
						auto activeEdge = CreateActive(edges[edgeIndex], scanY);

						// find insertion point
						if (active == std::list<ActiveEdge>::iterator())
						{
							active = activeEdges.insert(activeEdges.end(), activeEdge);
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
							for (++itr; itr != activeEdges.end(); ++itr)
							{
								if (itr->x >= activeEdge.x)
								{
									break;
								}
							}

							// at this point, p->next->x is NOT < z->x
							activeEdges.insert(itr, activeEdge);
						}
					}
					edgeIndex++;
				}

				// now process all active edges in non-zero fashion
				if (active != std::list<ActiveEdge>::iterator())
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


	void SvgRasterizer::Rasterize(const SvgDocument& document,
	                              unsigned char* outputBuffer,
	                              std::size_t width,
	                              std::size_t height,
	                              std::size_t stride,
	                              double tx, double ty,
	                              double scale)
	{
		auto& viewBox = document.GetElement().GetViewBox();
		auto& defs = document.GetDefs();

		std::vector<Edge> edges;
		edges.reserve(document.GetElement().GetChildren().size() * 4);

		std::memset(outputBuffer, 0, width * 4);

		std::unique_ptr<unsigned char[]> scanLine(new unsigned char[width]);
		for (auto& element : document.GetElement().GetChildren())
		{
			if (const auto shape = dynamic_cast<SvgPath*>(element.get()))
			{
				if (!shape->IsVisible()) continue;
				if (shape->GetFill().has_value())
				{
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

					auto paint = GetPaint(defs, shape->GetFill().value(), shape->GetOpacity());

					RasterizeSortedEdges(scanLine.get(), outputBuffer, width, stride, height, tx, ty, scale, edges, paint, SvgFillRule::NonZero, 5);
				}

				if (shape->GetStroke().has_value())
				{
				}
			}
		}

		UnpremultiplyAlpha(outputBuffer, width, height, stride);
	}
} // Elpida
