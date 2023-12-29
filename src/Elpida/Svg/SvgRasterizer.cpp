//
// Created by klapeto on 24/12/2023.
//

#include "Elpida/Svg/SvgRasterizer.hpp"

#include <algorithm>
#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgPath.hpp>
#include <Elpida/Svg/SvgShape.hpp>

namespace Elpida
{
	struct Edge
	{
		double x0, y0, x1, y1;
		int direction;
	};

	struct RasterizerPoint
	{
		double x, y;
		double dx, dy;
		double len;
		double dmx, dmy;
		unsigned char flags;
	};

	static constexpr double tessTol = 0.25;
	static constexpr double distTol = 0.01;

	static double AbsF(const double x) { return x < 0 ? -x : x; }
	static double RoundF(const double x) { return (x >= 0) ? floor(x + 0.5) : ceil(x - 0.5); }

	static int PointEquals(const double x1, const double y1, const double x2, const double y2)
	{
		const double dx = x2 - x1;
		const double dy = y2 - y1;
		return dx * dx + dy * dy < distTol * distTol;
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
			.y = y
		});
	}

	static void FlattenCubicBez(std::vector<RasterizerPoint>& points,
	                            const double x1, const double y1, const double x2, const double y2,
	                            const double x3, const double y3, const double x4, const double y4,
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
		const double d2 = AbsF(((x2 - x4) * dy - (y2 - y4) * dx));
		const double d3 = AbsF(((x3 - x4) * dy - (y3 - y4) * dx));

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

	static void FlatenShape(const SvgPath& path, std::vector<Edge>& edges,
	                        std::vector<RasterizerPoint>& points,
	                        const double scale)
	{
		for (auto& pathInstance : path.GetInstances())
		{
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
			for (std::size_t i = 0, j = points.size() - 1; i <points.size(); j = i++)
			{
				const auto& a = points[j];
				const auto& b = points[i];
				AddEdge(edges, a.x, a.y, b.x, b.y);
			}
		}
	}

	static void GetPaint(const SvgShape& shape, float opacity)
	{

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
		std::vector<RasterizerPoint> points;
		edges.reserve(document.GetElement().GetChildren().size() * 4);
		for (auto& element : document.GetElement().GetChildren())
		{
			if (auto shape = dynamic_cast<SvgPath*>(element.get()))
			{
				if (!shape->IsVisible()) continue;
				if (shape->GetFill().has_value())
				{
					FlatenShape(*shape, edges, points, scale);

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
						if (a.y0 < b.y0) return -1;
						if (a.y0 > b.y0) return 1;
						return 0;
					});

					// now, traverse the scanlines and find the intersections on each scanline, use non-zero rule
				}

				if (shape->GetStroke().has_value())
				{
				}
			}
		}
	}
} // Elpida
