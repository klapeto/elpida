//
// Created by klapeto on 5/3/2024.
//

#include "Elpida/Svg/SvgShapePolygonizer.hpp"

#include <Elpida/Svg/SvgEdge.hpp>
#include <Elpida/Svg/SvgLineCap.hpp>
#include <Elpida/Svg/SvgLineJoin.hpp>
#include <Elpida/Svg/SvgStroke.hpp>
#include <Elpida/Svg/SvgCalculatedShape.hpp>
#include <Elpida/Svg/SvgCalculationContext.hpp>
#include <Elpida/Svg/SvgUtilities.hpp>

namespace Elpida
{
	enum PointFlags
	{
		NONE = 0,
		CORNER = 0x01,
		BEVEL = 0x02,
		LEFT = 0x04
	};

	static constexpr SvgFloat tessTol = 0.25;

	class SvgRasterizerPoint : public SvgPoint
	{
	public:
		const SvgPoint& GetDirection() const
		{
			return _direction;
		}

		const SvgPoint& GetExtrusion() const
		{
			return _extrusion;
		}

		SvgPoint& GetExtrusion()
		{
			return _extrusion;
		}

		void SetDirection(const SvgPoint& direction)
		{
			_direction = direction;
		}

		void SetExtrusion(const SvgPoint& extrusion)
		{
			_extrusion = extrusion;
		}

		[[nodiscard]]
		PointFlags GetFlags() const
		{
			return _flags;
		}

		void AppendFlag(const PointFlags flag)
		{
			_flags = static_cast<PointFlags>(_flags | flag);
		}

		bool HasFlag(const PointFlags flag) const
		{
			return _flags & flag;
		}

		void SetFlag(const PointFlags flag)
		{
			_flags = flag;
		}

		SvgRasterizerPoint()
				:SvgPoint(), _flags(NONE)
		{
		}

		SvgRasterizerPoint(const SvgFloat x, const SvgFloat y)
				:SvgPoint(x, y), _flags(NONE)
		{
		}

		SvgRasterizerPoint(const SvgPoint& point, const PointFlags flags)
				:SvgPoint(point), _flags(flags)
		{
		}

		explicit SvgRasterizerPoint(const SvgPoint& point)
				:SvgPoint(point), _flags(NONE)
		{
		}

		SvgRasterizerPoint(const SvgFloat x,
				const SvgFloat y,
				const SvgFloat dx,
				const SvgFloat dy,
				const SvgFloat dmx,
				const SvgFloat dmy,
				const PointFlags flags)
				:SvgPoint(x, y),
				 _direction(dx, dy),
				 _extrusion(dmx, dmy),
				 _flags(flags)
		{
		}

		SvgRasterizerPoint(const SvgRasterizerPoint&) = default;
		SvgRasterizerPoint& operator=(const SvgRasterizerPoint&) = default;
		SvgRasterizerPoint(SvgRasterizerPoint&&) noexcept = default;
		SvgRasterizerPoint& operator=(SvgRasterizerPoint&&) noexcept = default;

	private:
		SvgPoint _direction;
		SvgPoint _extrusion;
		PointFlags _flags;
	};

	static int CurveDivs(const SvgFloat r, const SvgFloat arc)
	{
		const SvgFloat da = std::acos(r / (r + tessTol)) * SvgFloat(2.0);
		int divs = static_cast<int>(std::ceil(arc / da));
		if (divs < 2) divs = 2;
		return divs;
	}

	static void AddPoint(std::vector<SvgRasterizerPoint>& points, const SvgPoint& point, const PointFlags flags)
	{
		if (!points.empty())
		{
			auto& lastPoint = points.back();
			if (lastPoint == point)
			{
				lastPoint.AppendFlag(flags);
				return;
			}
		}

		points.emplace_back(point, flags);
	}

	static void AddEdge(std::vector<SvgEdge>& edges, const SvgPoint& a, const SvgPoint& b)
	{
		// Skip horizontal edges
		if (a.GetY() == b.GetY())
		{
			return;
		}

		if (a.GetY() < b.GetY())
		{
			edges.emplace_back(a, b, 1);
		}
		else
		{
			edges.emplace_back(b, a, -1);
		}
	}

	static void InitClosed(SvgPoint& left,
			SvgPoint& right,
			const SvgPoint& p0,
			const SvgPoint& p1,
			const SvgFloat lineWidth)
	{
		const SvgFloat w = lineWidth * SvgFloat(0.5);

		auto delta = p1 - p0;
		const auto length = delta.Normalize();

		const auto p = p0 + (delta * (length * SvgFloat(0.5)));

		const auto inverseDelta = delta.GetInverse();

		left = p - (inverseDelta * w);
		right = p + (inverseDelta * w);
	}

	static void ButtCap(std::vector<SvgEdge>& edges,
			SvgPoint& left,
			SvgPoint& right,
			const SvgPoint& p,
			const SvgPoint& delta,
			const SvgFloat lineWidth,
			const bool connect)
	{
		const SvgFloat w = lineWidth * SvgFloat(0.5);

		const auto inverseDelta = delta.GetInverse();

		const SvgPoint thisLeft = p - (inverseDelta * w);
		const SvgPoint thisRight = p + (inverseDelta * w);

		AddEdge(edges, thisLeft, thisRight);

		if (connect)
		{
			AddEdge(edges, left, thisLeft);
			AddEdge(edges, thisRight, right);
		}
		left = thisLeft;
		right = thisRight;
	}

	static void SquareCap(std::vector<SvgEdge>& edges,
			SvgPoint& left,
			SvgPoint& right,
			const SvgPoint& p,
			const SvgPoint& delta,
			const SvgFloat lineWidth,
			const bool connect)
	{
		const SvgFloat w = lineWidth * SvgFloat(0.5);

		const auto newPoint = p - delta * w;

		const auto inverseDelta = delta.GetInverse();

		const SvgPoint thisLeft = newPoint - (inverseDelta * w);
		const SvgPoint thisRight = newPoint + (inverseDelta * w);

		AddEdge(edges, thisLeft, thisRight);

		if (connect)
		{
			AddEdge(edges, left, thisLeft);
			AddEdge(edges, thisRight, right);
		}
		left = thisLeft;
		right = thisRight;
	}

	static void RoundCap(std::vector<SvgEdge>& edges,
			SvgPoint& left,
			SvgPoint& right,
			const SvgPoint& p,
			const SvgPoint& delta,
			const SvgFloat lineWidth,
			const std::size_t ncap,
			const bool connect)
	{
		const SvgFloat w = lineWidth * SvgFloat(0.5);

		const auto inverseDelta = delta.GetInverse();

		SvgPoint thisLeft;
		SvgPoint thisRight;
		SvgPoint previousPoint;

		for (std::size_t i = 0; i < ncap; i++)
		{
			const SvgFloat a = static_cast<SvgFloat>(i) / static_cast<SvgFloat>(ncap - 1) * SvgUtilities::Pi;
			const SvgFloat ax = std::cos(a) * w;
			const SvgFloat ay = std::sin(a) * w;

			auto currentPoint = p - (inverseDelta * ax) - (delta * ay);

			if (i > 0)
			{
				AddEdge(edges, previousPoint, currentPoint);
			}

			previousPoint = currentPoint;

			if (i == 0)
			{
				thisLeft = currentPoint;
			}
			else if (i == ncap - 1)
			{
				thisRight = currentPoint;
			}
		}

		if (connect)
		{
			AddEdge(edges, left, thisLeft);
			AddEdge(edges, thisRight, right);
		}

		left = thisLeft;
		right = thisRight;
	}

	static void BevelJoin(std::vector<SvgEdge>& edges,
			SvgPoint& left,
			SvgPoint& right,
			const SvgRasterizerPoint& p0,
			const SvgRasterizerPoint& p1,
			const SvgFloat lineWidth)
	{
		const SvgFloat w = lineWidth * SvgFloat(0.5);
		const auto dl0 = p0.GetDirection().GetInverse();
		const auto dl1 = p1.GetDirection().GetInverse();

		const auto leftA = p1 - (dl0 * w);
		const auto rightA = p1 + (dl0 * w);

		const auto leftB = p1 - (dl1 * w);
		const auto rightB = p1 + (dl1 * w);

		AddEdge(edges, leftA, left);
		AddEdge(edges, leftB, leftA);

		AddEdge(edges, right, rightA);
		AddEdge(edges, rightA, rightB);

		left = leftB;
		right = rightB;
	}

	static void MiterJoin(std::vector<SvgEdge>& edges,
			SvgPoint& left,
			SvgPoint& right,
			const SvgRasterizerPoint& p0,
			const SvgRasterizerPoint& p1,
			const SvgFloat lineWidth)
	{
		const SvgFloat w = lineWidth * SvgFloat(0.5);

		const auto dl0 = p0.GetDirection().GetInverse();
		const auto dl1 = p1.GetDirection().GetInverse();

		SvgPoint left1;
		SvgPoint right1;

		if (p1.HasFlag(PointFlags::LEFT))
		{
			left1 = p1 - (p1.GetExtrusion() * w);

			AddEdge(edges, left1, left);

			const auto right0 = p1 + (dl0 * w);
			right1 = p1 + (dl1 * w);

			AddEdge(edges, right, right0);
			AddEdge(edges, right0, right1);
		}
		else
		{
			const auto left0 = p1 - (dl0 * w);
			left1 = p1 - (dl1 * w);

			AddEdge(edges, left0, left);
			AddEdge(edges, left1, left0);

			right1 = p1 + (p1.GetExtrusion() * w);
			AddEdge(edges, right, right1);
		}

		left = left1;
		right = right1;
	}

	static void RoundJoin(std::vector<SvgEdge>& edges,
			SvgPoint& left,
			SvgPoint& right,
			const SvgRasterizerPoint& p0,
			const SvgRasterizerPoint& p1,
			const SvgFloat lineWidth,
			const std::size_t ncap)
	{
		const SvgFloat w = lineWidth * SvgFloat(0.5);
		const auto dl0 = p0.GetDirection().GetInverse();
		const auto dl1 = p1.GetDirection().GetInverse();
		const SvgFloat a0 = std::atan2(dl0.GetY(), dl0.GetX());
		const SvgFloat a1 = std::atan2(dl1.GetY(), dl1.GetX());
		SvgFloat da = a1 - a0;

		if (da < SvgUtilities::Pi) da += SvgUtilities::Pi * 2;
		if (da > SvgUtilities::Pi) da -= SvgUtilities::Pi * 2;

		auto nV = static_cast<int>(std::ceil((std::abs(da) / SvgUtilities::Pi) * static_cast<SvgFloat>(ncap)));
		if (nV < 2) nV = 2;

		std::size_t n = nV;
		if (n > ncap) n = ncap;

		SvgPoint left0 = left;
		SvgPoint right0 = right;

		for (std::size_t i = 0; i < n; i++)
		{
			const SvgFloat u = static_cast<SvgFloat>(i) / static_cast<SvgFloat>(n - 1);
			const SvgFloat a = a0 + u * da;
			SvgPoint angle(std::cos(a) * w, std::sin(a) * w);

			auto left1 = p1 - angle;
			auto right1 = p1 + angle;

			AddEdge(edges, left1, left0);
			AddEdge(edges, right0, right1);

			left0 = left1;
			right0 = right1;
		}

		left = left0;
		right = right0;
	}

	static void StraightJoin(std::vector<SvgEdge>& edges,
			SvgPoint& left,
			SvgPoint& right,
			const SvgRasterizerPoint& p1,
			const SvgFloat lineWidth)
	{
		const SvgFloat w = lineWidth * SvgFloat(0.5);
		const auto left0 = p1 - (p1.GetExtrusion() * w);
		const auto right0 = p1 + (p1.GetExtrusion() * w);

		AddEdge(edges, left0, left);
		AddEdge(edges, right, right0);

		left = left0;
		right = right0;
	}

	static void FlattenCubicBez(std::vector<SvgRasterizerPoint>& points,
			const SvgPoint& start,
			const SvgPoint& controlA,
			const SvgPoint& controlB,
			const SvgPoint& end,
			const int level,
			const PointFlags type)
	{
		if (level > 10)
		{
			return;
		}

		const SvgFloat dx = end.GetX() - start.GetX();
		const SvgFloat dy = end.GetY() - start.GetY();
		const SvgFloat d2 = std::abs(((controlA.GetX() - end.GetX()) * dy) - ((controlA.GetY() - end.GetY()) * dx));
		const SvgFloat d3 = std::abs(((controlB.GetX() - end.GetX()) * dy) - ((controlB.GetY() - end.GetY()) * dx));

		if ((d2 + d3) * (d2 + d3) < tessTol * (dx * dx + dy * dy))
		{
			AddPoint(points, end, type);
			return;
		}

		// We split the curve into 2
		const auto p1p2 = (start + controlA) * 0.5;
		const auto p2p3 = (controlA + controlB) * 0.5;
		const auto p3p4 = (controlB + end) * 0.5;
		const auto p1p2p3 = (p1p2 + p2p3) * 0.5;
		const auto p2p3p4 = (p2p3 + p3p4) * 0.5;
		const auto p1p2p3p4 = (p1p2p3 + p2p3p4) * 0.5; // Midpoint

		FlattenCubicBez(points, start, p1p2, p1p2p3, p1p2p3p4, level + 1, PointFlags::NONE);
		FlattenCubicBez(points, p1p2p3p4, p2p3p4, p3p4, end, level + 1, type);
	}

	static void ExpandStroke(std::vector<SvgEdge>& edges,
			std::vector<SvgRasterizerPoint>& points,
			bool closed,
			const SvgLineJoin lineJoin,
			const SvgLineCap lineCap,
			SvgFloat lineWidth)
	{
		const auto ncap = CurveDivs(lineWidth * SvgFloat(0.5), SvgUtilities::Pi);
		SvgRasterizerPoint left, right, firstLeft, firstRight;

		std::vector<SvgRasterizerPoint>::iterator p0;
		std::vector<SvgRasterizerPoint>::iterator p1;

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
			auto delta = *p1 - *p0;
			delta.Normalize();
			switch (lineCap)
			{
			case SvgLineCap::Butt:
				ButtCap(edges, left, right, *p0, delta, lineWidth, false);
				break;
			case SvgLineCap::Round:
				RoundCap(edges, left, right, *p0, delta, lineWidth, ncap, false);
				break;
			case SvgLineCap::Square:
				SquareCap(edges, left, right, *p0, delta, lineWidth, false);
				break;
			}
		}

		for (std::size_t i = s; i < e; ++i)
		{
			if (p1->HasFlag(PointFlags::CORNER))
			{
				if (lineJoin == SvgLineJoin::Round)
				{
					RoundJoin(edges, left, right, *p0, *p1, lineWidth, ncap);
				}
				else if (lineJoin == SvgLineJoin::Bevel || (p1->HasFlag(PointFlags::BEVEL)))
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
			AddEdge(edges, firstLeft, left);
			AddEdge(edges, right, firstRight);
		}
		else
		{
			// Add cap
			auto delta = *p1 - *p0;
			delta.Normalize();

			if (lineCap == SvgLineCap::Butt)
			{
				ButtCap(edges, right, left, *p1, -delta, lineWidth, true);
			}
			else if (lineCap == SvgLineCap::Square)
			{
				SquareCap(edges, right, left, *p1, -delta, lineWidth, true);
			}
			else if (lineCap == SvgLineCap::Round)
			{
				RoundCap(edges, right, left, *p1, -delta, lineWidth, ncap, true);
			}
		}
	}

	static void PrepareStroke(std::vector<SvgRasterizerPoint>& points,
			const SvgFloat miterLimit,
			const SvgLineJoin lineJoin)
	{
		// TODO: index based
		auto p0 = --points.end();
		auto p1 = points.begin();

		for (std::size_t i = 0; i < points.size(); ++i)
		{
			// Calculate segment direction and length
			SvgPoint direction = *p1 - *p0;
			direction.Normalize();
			p0->SetDirection(direction);

			// Advance
			p0 = p1++;
		}

		// calculate joins
		p0 = --points.end();
		p1 = points.begin();

		for (std::size_t i = 0; i < points.size(); ++i)
		{
			auto dl0 = p0->GetDirection().GetInverse();
			auto dl1 = p1->GetDirection().GetInverse();

			// Calculate extrusions
			p1->SetExtrusion((dl0 + dl1) * 0.5);

			const SvgFloat dmr2 = p1->GetExtrusion().Product();
			if (dmr2 > 0.000001)
			{
				SvgFloat s2 = SvgFloat(1.0) / dmr2;
				if (s2 > 600.0)
				{
					s2 = 600.0;
				}
				p1->GetExtrusion() *= s2;
			}

			// Clear flags, but keep the corner.
			if (p1->HasFlag(PointFlags::CORNER))
			{
				p1->SetFlag(PointFlags::CORNER);
			}
			else
			{
				p1->SetFlag(NONE);
			}

			// Keep track of left turns.
			const auto cross =
					p1->GetDirection().GetX() * p0->GetDirection().GetY() -
					p0->GetDirection().GetX() * p1->GetDirection().
							GetY();
			if (cross > 0.0)
			{
				p1->AppendFlag(LEFT);
			}

			// Check to see if the corner needs to be beveled.
			if (p1->HasFlag(CORNER))
			{
				if ((dmr2 * miterLimit * miterLimit) < 1.0
					|| lineJoin == SvgLineJoin::Bevel
					|| lineJoin == SvgLineJoin::Round)
				{
					p1->AppendFlag(BEVEL);
				}
			}

			p0 = p1++;
		}
	}

	SvgPolygon SvgShapePolygonizer::Polygonize(const SvgCalculatedShape& shape)
	{
		std::vector<SvgEdge> edges;

		for (auto& pathInstance : shape.GetPaths())
		{
			std::vector<SvgRasterizerPoint> points;

			auto& curves = pathInstance.GetCurves();
			auto startPoint = pathInstance.GetStartPoint();
			// Flatten path
			AddPoint(points, startPoint, PointFlags::NONE);

			for (auto& curve : curves)
			{
				auto endPoint = curve.GetEnd();
				FlattenCubicBez(points,
						startPoint,
						curve.GetStartControl(),
						curve.GetEndControl(),
						endPoint,
						0,
						PointFlags::NONE);
				startPoint = endPoint;
			}

			// Close path
			AddPoint(points, startPoint, PointFlags::NONE);

			// Build edges
			for (std::size_t i = 0, j = points.size() - 1; i < points.size(); j = i++)
			{
				AddEdge(edges, points[j], points[i]);
			}
		}

		return SvgPolygon(std::move(edges));
	}

	SvgPolygon SvgShapePolygonizer::PolygonizeStroke(const SvgCalculatedShape& shape)
	{
		if (!shape.GetStroke().has_value()) return {};

		std::vector<SvgEdge> edges;

		constexpr SvgFloat scale = 1.0;

		auto& stroke = shape.GetStroke().value();
		const SvgFloat lineWidth = stroke.GetWidth() * scale;
		for (auto& path : shape.GetPaths())
		{
			auto& curves = path.GetCurves();
			if (curves.empty()) continue;

			std::vector<SvgRasterizerPoint> points;

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
						0,
						PointFlags::CORNER);

				startPoint = endPoint;
			}

			if (points.size() < 2)
			{
				continue;
			}

			bool closed = path.IsClosed();

			// If the first and last points are the same, remove the last, mark as closed path.
			if (points.front() == points.back())
			{
				points.pop_back();
				closed = true;
			}

			auto& dashesArray = stroke.GetDashes();
			if (!dashesArray.empty())
			{
				auto& calculatedDashes = stroke.GetDashes();
				bool dashState = true;
				if (closed)
				{
					points.push_back(points.front());
				}

				// Duplicate points -> points2.
				auto points2 = points;

				points.clear();

				auto current = points2.front();
				points.push_back(current);

				// Figure out dash offset.
				SvgFloat allDashLength = 0.0;
				for (const auto dash : calculatedDashes)
				{
					allDashLength += dash;
				}

				if (calculatedDashes.size() & 1)
				{
					allDashLength *= 2.0;
				}

				// Find location inside pattern
				auto dashOffset = std::fmod(stroke.GetDashOffset(), allDashLength);
				if (dashOffset < 0.0)
				{
					dashOffset += allDashLength;
				}

				std::size_t dashIndex = 0;
				while (dashOffset > calculatedDashes[dashIndex])
				{
					dashOffset -= calculatedDashes[dashIndex];
					dashIndex = (dashIndex + 1) % calculatedDashes.size();
				}

				auto dashLength = (calculatedDashes[dashIndex] - dashOffset) * scale;
				SvgFloat totalDistance = 0.0;
				for (std::size_t j = 1; j < points2.size();)
				{
					auto delta = points2[j] - current;
					const SvgFloat distance = std::sqrt((delta.GetX() * delta.GetX()) + (delta.GetY() * delta.GetY()));

					if ((totalDistance + distance) > dashLength)
					{
						// Calculate intermediate point
						const SvgFloat d = (dashLength - totalDistance) / distance;

						auto point = current + (delta * d);

						AddPoint(points, point, PointFlags::CORNER);

						// Stroke
						if (points.size() > 1 && dashState)
						{
							PrepareStroke(points, stroke.GetMiterLimit(), stroke.GetLineJoin());
							ExpandStroke(edges, points, false, stroke.GetLineJoin(), stroke.GetLineCap(), lineWidth);
						}

						// Advance dash pattern
						dashState = !dashState;
						dashIndex = (dashIndex + 1) % calculatedDashes.size();
						dashLength = calculatedDashes[dashIndex] * scale;

						// Restart
						current = SvgRasterizerPoint(point, CORNER);
						totalDistance = 0.0;
						points.clear();
						points.push_back(current);
					}
					else
					{
						totalDistance += distance;
						current = points2[j];
						points.push_back(current);
						j++;
					}
				}

				// Stroke any leftover path
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

		return SvgPolygon(std::move(edges));
	}
} // Elpida
