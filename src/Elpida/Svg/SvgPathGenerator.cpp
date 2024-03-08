//
// Created by klapeto on 23/1/2024.
//

#include "Elpida/Svg/SvgPathGenerator.hpp"

#include <Elpida/Svg/SvgNumber.hpp>
#include <Elpida/Xml/CharacterStream.hpp>
#include <Elpida/Xml/ParseException.hpp>

namespace Elpida
{
	static double Square(const double x)
	{
		return x * x;
	}

	static bool IsNumberic(const char c)
	{
		return SvgNumber::IsNumber(c) || c == '+' || c == '-';
	}

	static double VectorMagnitude(const double x, const double y)
	{
		return sqrt(x * x + y * y);
	}

	static double VectorDotProduct(const double x1, const double y1, const double x2, const double y2)
	{
		return x1 * x2 + y1 * y2;
	}

	static double VectorCosTheta(const double x1, const double y1, const double x2, const double y2)
	{
		return VectorDotProduct(x1, y2, x2, y2) / (VectorMagnitude(x1, y1) * VectorMagnitude(x2, y2));
	}

	static double VectorAngle(const double x1, const double y1, const double x2, const double y2)
	{
		double res = VectorCosTheta(x1, y1, x2, y2);
		if (res < -1.0) res = -1.0;
		if (res > 1.0) res = 1.0;
		return (x1 * y2 < y1 * x2 ? -1.0 : 1.0) * acos(res);
	}

	static void ParseAllNumbers(CharacterStream& stream, std::vector<double>& currentNumbers)
	{
		currentNumbers.clear();
		stream.Next();
		while (!stream.Eof())
		{
			stream.SkipSpace();
			currentNumbers.push_back(SvgNumber::ParseNumber(stream.GetStringViewWhile([](auto c)
			{
				return IsNumberic(c) || c == '.';
			})));
			stream.SkipSpace();
			if (IsNumberic(stream.Current()))
			{
				continue;
			}
			if (stream.Current() == ',')
			{
				stream.Next();
			}
			else
			{
				break;
			}
		}
	}

	void SvgPathGenerator::ParsePathData(const std::string& data)
	{
		if (data.empty()) return;
		CharacterStream stream(data);

		try
		{
			std::vector<double> currentArguments;
			currentArguments.reserve(8);

			while (!stream.Eof())
			{
				stream.SkipSpace();
				bool relative = false;
				switch (const auto c = stream.Current())
				{
				case 'm':
					relative = true;
					[[fallthrough]];
				case 'M':
				{
					CommitPath(_closed);
					_closed = false;
					ParseAllNumbers(stream, currentArguments);
					if (currentArguments.empty() || currentArguments.size() % 2 != 0)
					{
						throw ParseException("moveTo command requires x,y");
					}

					MoveTo(SvgPoint(currentArguments[0], currentArguments[1]), relative);

					for (std::size_t i = 2; i < currentArguments.size(); i += 2)
					{
						LineTo(SvgPoint(currentArguments[i], currentArguments[i + 1]), relative);
					}
				}

				break;
				case 'l':
					relative = true;
					[[fallthrough]];
				case 'L':
				{
					ParseAllNumbers(stream, currentArguments);
					if (currentArguments.empty() || currentArguments.size() % 2 != 0)
					{
						throw ParseException("lineTo command requires x,y");
					}

					for (std::size_t i = 0; i < currentArguments.size(); i += 2)
					{
						LineTo(SvgPoint(currentArguments[i], currentArguments[i + 1]), relative);
					}
				}
				break;
				case 'h':
					relative = true;
					[[fallthrough]];
				case 'H':
				{
					ParseAllNumbers(stream, currentArguments);

					if (currentArguments.empty())
					{
						throw ParseException("horizontalLineTo command requires x");
					}

					for (std::size_t i = 0; i < currentArguments.size(); i++)
					{
						HorizontalLineTo(currentArguments[i], relative);
					}
				}
				break;
				case 'v':
					relative = true;
					[[fallthrough]];
				case 'V':
				{
					ParseAllNumbers(stream, currentArguments);

					if (currentArguments.empty())
					{
						throw ParseException("verticallLineTo command requires x");
					}

					for (std::size_t i = 0; i < currentArguments.size(); i++)
					{
						VerticalLineTo(currentArguments[i], relative);
					}
				}
				break;
				case 'c':
					relative = true;
					[[fallthrough]];
				case 'C':
				{
					ParseAllNumbers(stream, currentArguments);

					if (currentArguments.empty() || currentArguments.size() % 6 != 0)
					{
						throw ParseException("curveTo command requires x1 y1 x2 y2 x y");
					}

					for (std::size_t i = 0; i < currentArguments.size(); i += 6)
					{
						CubicBezTo(
							SvgPoint(currentArguments[i], currentArguments[i + 1]),
							SvgPoint(currentArguments[i + 2], currentArguments[i + 3]),
							SvgPoint(currentArguments[i + 4], currentArguments[i + 5]),
							relative);
					}
				}
				break;
				case 's':
					relative = true;
					[[fallthrough]];
				case 'S':
				{
					ParseAllNumbers(stream, currentArguments);

					if (currentArguments.empty() || currentArguments.size() % 4 != 0)
					{
						throw ParseException("smoothCurveTo command requires x2 y2 x y");
					}

					for (std::size_t i = 0; i < currentArguments.size(); i += 4)
					{
						CubicBezShortTo(
							SvgPoint(currentArguments[i], currentArguments[i + 1]),
							SvgPoint(currentArguments[i + 2], currentArguments[i + 3]),
							relative);
					}
				}
				break;
				case 'q':
					relative = true;
					[[fallthrough]];
				case 'Q':
				{
					ParseAllNumbers(stream, currentArguments);

					if (currentArguments.empty() || currentArguments.size() % 4 != 0)
					{
						throw ParseException("quadricCurveTo command requires x2 y2 x y");
					}

					for (std::size_t i = 0; i < currentArguments.size(); i += 4)
					{
						QuadBezTo(
							SvgPoint(currentArguments[i], currentArguments[i + 1]),
							SvgPoint(currentArguments[i + 2],currentArguments[i + 3]),
							relative);
					}
				}
				break;
				case 't':
					relative = true;
					[[fallthrough]];
				case 'T':
				{
					ParseAllNumbers(stream, currentArguments);
					if (currentArguments.empty() || currentArguments.size() % 2 != 0)
					{
						throw ParseException("smoothQuadricCurveTo command requires x y");
					}

					for (std::size_t i = 0; i < currentArguments.size(); i += 2)
					{
						QuadBezShortTo(
							SvgPoint(currentArguments[i], currentArguments[i + 1]),
							relative);
					}
				}
				break;
				case 'a':
					relative = true;
					[[fallthrough]];
				case 'A':
				{
					ParseAllNumbers(stream, currentArguments);

					if (currentArguments.empty() || currentArguments.size() % 7 != 0)
					{
						throw ParseException(
							"arc command requires rx ry x-axis-rotation large-arc-flag sweep-flag x y");
					}

					for (std::size_t i = 0; i < currentArguments.size(); i += 7)
					{
						ArcTo(
							currentArguments[i],
							currentArguments[i + 1],
							currentArguments[i + 2],
							fabs(currentArguments[i + 3]) > 1e-6,
							fabs(currentArguments[i + 4]) > 1e-6,
							SvgPoint(currentArguments[i + 5], currentArguments[i + 6]),
							relative);
					}
				}
				break;
				case 'Z':
					[[fallthrough]];
				case 'z':
					_closed = true;
					if (!_curves.empty())
					{
						_currentPoint = _moveToPoint;
						_previousControlPointB = _currentPoint;
						CommitPath(_closed);
					}
					_closed = false;
					_moveToPoint = _currentPoint;
					stream.Next();
					break;
				default:
					throw ParseException(c, "m,M,");
				}
			}
			CommitPath(_closed);
		}
		catch (const ParseException&)
		{
			_paths.clear();
		}
	}

	void SvgPathGenerator::MoveTo(const SvgPoint& point, const bool relative)
	{
		CommitPath(_closed);
		if (relative)
		{
			_moveToPoint += point;
		}
		else
		{
			_moveToPoint = point;
		}

		_currentPoint = _moveToPoint;
		_previousControlPointB = _currentPoint;
	}

	void SvgPathGenerator::CubicBezTo(SvgPoint controlPointA, SvgPoint controlPointB, SvgPoint endPoint, const bool relative)
	{
		if (relative)
		{
			controlPointA += _currentPoint;
			controlPointB += _currentPoint;
			endPoint += _currentPoint;
		}

		_curves.emplace_back(controlPointA, controlPointB, endPoint);

		_previousControlPointB = controlPointB;
		_currentPoint = endPoint;
	}

	void SvgPathGenerator::CubicBezShortTo(SvgPoint controlPointB, SvgPoint endPoint,const bool relative)
	{
		if (relative)
		{
			controlPointB += _currentPoint;
			endPoint += _currentPoint;
		}

		const SvgPoint controlPointA(2 * _currentPoint.GetX() - controlPointB.GetX(),
			2 * _currentPoint.GetY() - controlPointB.GetY());

		_curves.emplace_back(controlPointA, controlPointB, endPoint);

		_previousControlPointB = controlPointB;
		_currentPoint = endPoint;
	}

	void SvgPathGenerator::QuadBezTo(SvgPoint controlPoint, SvgPoint endPoint,  const bool relative)
	{
		if (relative)
		{
			controlPoint += _currentPoint;
			endPoint += _currentPoint;
		}

		// Convert to cubic bezier
		const SvgPoint controlPointA(
			_currentPoint.GetX() + 2.0 / 3.0 * (controlPoint.GetX() - _currentPoint.GetX()),
			_currentPoint.GetY() + 2.0 / 3.0 * (controlPoint.GetY() - _currentPoint.GetY()));

		const SvgPoint controlPointB(
			endPoint.GetX() + 2.0 / 3.0 * (controlPoint.GetX() - endPoint.GetX()),
			endPoint.GetY() + 2.0 / 3.0 * (controlPoint.GetY() - endPoint.GetY()));

		_curves.emplace_back(controlPointA, controlPointB, endPoint);

		_previousControlPointB = controlPointB;
		_currentPoint = endPoint;
	}

	void SvgPathGenerator::QuadBezShortTo(SvgPoint endPoint, const bool relative)
	{
		if (relative)
		{
			endPoint += _currentPoint;
		}

		const double cx = 2 * _currentPoint.GetX() - _previousControlPointB.GetX();
		const double cy = 2 * _currentPoint.GetY() - _previousControlPointB.GetY();

		const SvgPoint controlPointA(
			_currentPoint.GetX() + 2.0 / 3.0 * (cx - _currentPoint.GetX()),
			_currentPoint.GetY() + 2.0 / 3.0 * (cy - _currentPoint.GetY())
		);

		const SvgPoint controlPointB(
			endPoint.GetX() + 2.0 / 3.0 * (cx - endPoint.GetX()),
			endPoint.GetY() + 2.0 / 3.0 * (cy - endPoint.GetY())
		);

		_curves.emplace_back(controlPointA, controlPointB, endPoint);

		_previousControlPointB = controlPointB;
		_currentPoint = endPoint;
	}

	void SvgPathGenerator::ArcTo(
		const double radiusX,
		const double radiusY,
		const double xAxisRotation,
		const bool largeArc,
		const bool sweep,
		SvgPoint endpoint,
		const bool relative)
	{
		auto rx = fabs(radiusX); // y radius
		auto ry = fabs(radiusY); // x radius
		const auto rotx = xAxisRotation / 180.0 * std::numbers::pi; // x rotation angle
		const auto fa = largeArc; // Large arc
		const auto fs = sweep; // Sweep direction

		if (relative)
		{
			endpoint += _currentPoint;
		}

		double dx = _currentPoint.GetX() - endpoint.GetX();
		double dy = _currentPoint.GetY() - endpoint.GetY();
		double distance = sqrt(dx * dx + dy * dy);
		if (distance < 1e-6 || rx < 1e-6 || ry < 1e-6)
		{
			// The arc degenerates to a line
			LineTo(_currentPoint, endpoint);
			_currentPoint = endpoint;
			_previousControlPointB = endpoint;
			return;
		}

		const auto sinrx = sin(rotx);
		const auto cosrx = cos(rotx);

		// Convert to center point parameterization.
		// http://www.w3.org/TR/SVG11/implnote.html#ArcImplementationNotes
		// 1) Compute x1', y1'
		const double x1p = cosrx * dx / 2.0 + sinrx * dy / 2.0;
		const double y1p = -sinrx * dx / 2.0 + cosrx * dy / 2.0;
		distance = Square(x1p) / Square(rx) + Square(y1p) / Square(ry);
		if (distance > 1)
		{
			distance = sqrt(distance);
			rx *= distance;
			ry *= distance;
		}

		// 2) Compute cx', cy'
		double s = 0.0;
		double sa = Square(rx) * Square(ry) - Square(rx) * Square(y1p) - Square(ry) * Square(x1p);
		const double sb = Square(rx) * Square(y1p) + Square(ry) * Square(x1p);
		if (sa < 0.0)
		{
			sa = 0.0;
		}

		if (sb > 0.0)
		{
			s = sqrt(sa / sb);
		}

		if (fa == fs)
		{
			s = -s;
		}

		const double cxp = s * rx * y1p / ry;
		const double cyp = s * -ry * x1p / rx;

		// 3) Compute cx,cy from cx',cy'
		const double cx = (_currentPoint.GetX() + endpoint.GetX()) / 2.0 + cosrx * cxp - sinrx * cyp;
		const double cy = (_currentPoint.GetY() + endpoint.GetY()) / 2.0 + sinrx * cxp + cosrx * cyp;

		// 4) Calculate theta1, and delta theta.
		const double ux = (x1p - cxp) / rx;
		const double uy = (y1p - cyp) / ry;
		const double vx = (-x1p - cxp) / rx;
		const double vy = (-y1p - cyp) / ry;
		const double a1 = VectorAngle(1.0, 0.0, ux, uy); // Initial angle
		double da = VectorAngle(ux, uy, vx, vy); // Delta angle

		//	if (vecrat(ux,uy,vx,vy) <= -1.0f) da = NSVG_PI;
		//	if (vecrat(ux,uy,vx,vy) >= 1.0f) da = 0;

		if (fs == 0 && da > 0)
		{
			da -= 2 * std::numbers::pi;
		}
		else if (fs == 1 && da < 0)
		{
			da += 2 * std::numbers::pi;
		}

		const SvgTransform transform(cosrx, sinrx, -sinrx, cosrx, cx, cy);
		// Approximate the arc using cubic spline segments.

		// Split arc into max 90 degree segments.
		// The loop assumes an iteration per end point (including start and end), this +1.
		const auto ndivs = static_cast<std::size_t>(fabs(da) / (std::numbers::pi * 0.5) + 1.0);
		auto hda = (da / static_cast<double>(ndivs)) / 2.0;

		// Fix for ticket #179: division by 0: avoid cotangens around 0 (infinite)
		if (hda < 1e-3 && hda > -1e-3)
		{
			hda *= 0.5;
		}
		else
		{
			hda = (1.0 - cos(hda)) / sin(hda);
		}

		double kappa = fabs(4.0 / 3.0 * hda);

		if (da < 0.0)
		{
			kappa = -kappa;
		}

		double tanx;
		double tany;
		double x;
		double y;
		double px = 0;
		double py = 0;
		double ptanx = 0;
		double ptany = 0;
		for (std::size_t i = 0; i <= ndivs; i++)
		{
			const double a = a1 + da * (static_cast<double>(i) / static_cast<double>(ndivs));

			dx = cos(a);
			dy = sin(a);

			transform.ApplyToPoint(x, y, dx * rx, dy * ry); // position
			transform.ApplyToVector(tanx, tany, -dy * rx * kappa, dx * ry * kappa); // tangent

			if (i > 0)
			{
				CubicBezTo(SvgPoint(px + ptanx, py + ptany), SvgPoint(x - tanx, y - tany), SvgPoint(x, y));
			}

			px = x;
			py = y;
			ptanx = tanx;
			ptany = tany;
		}

		_currentPoint = endpoint;
		_previousControlPointB = endpoint;
	}

	void SvgPathGenerator::LineTo(const SvgPoint& point, const bool relative)
	{
		const auto initialStartPoint = _currentPoint;

		if (relative)
		{
			_currentPoint += point;
		}
		else
		{
			_currentPoint = point;
		}

		LineTo(initialStartPoint, _currentPoint);
		_previousControlPointB = _currentPoint;
	}

	void SvgPathGenerator::HorizontalLineTo(const double x, const bool relative)
	{
		const auto initialStartPoint = _currentPoint;
		if (relative)
		{
			_currentPoint.GetRefX() += x;
		}
		else
		{
			_currentPoint.GetRefX() = x;
		}
		LineTo(initialStartPoint, _currentPoint);
		_previousControlPointB = _currentPoint;
	}

	void SvgPathGenerator::VerticalLineTo(const double a, const bool relative)
	{
		const auto initialStartPoint = _currentPoint;
		if (relative)
		{
			_currentPoint.GetRefY() += a;
		}
		else
		{
			_currentPoint.GetRefY() = a;
		}
		LineTo(initialStartPoint, _currentPoint);
		_previousControlPointB = _currentPoint;
	}

	void SvgPathGenerator::LineTo(const SvgPoint& start, const SvgPoint& end)
	{
		const double px = start.GetX();
		const double py = start.GetY();
		const double x = end.GetX();
		const double y = end.GetY();
		const double dx = x - px;
		const double dy = y - py;
		_curves.emplace_back(SvgPoint(px + dx / 3.0, py + dy / 3.0),
			SvgPoint(x - dx / 3.0, y - dy / 3.0),
			end);
	}

	void SvgPathGenerator::CommitPath(bool closed)
	{
		if (_curves.empty())
		{
			return;
		}

		if (closed)
		{
			const auto& lastCurve = _curves.back();
			LineTo(lastCurve.GetEnd(), _moveToPoint);
		}

		// auto& transform = GetTransform();
		//
		auto start = _moveToPoint;
		// start.ApplyTransform(transform);
		//
		// for (auto& curve : curves)
		// {
		// 	curve.ApplyTransform(transform);
		// }

		const auto* currentCurve = &_curves.front();

		SvgBounds bounds = CalculateBounds(start, currentCurve->GetStartControl(), currentCurve->GetEndControl(), currentCurve->GetEnd());
		for (std::size_t i = 1; i < _curves.size() - 1; ++i)
		{
			auto& curve = _curves[i];
			bounds.Merge(CalculateBounds(currentCurve->GetEnd(),
				curve.GetStartControl(),
				curve.GetEndControl(),
				curve.GetEnd()));
			currentCurve = &curve;
		}

		_paths.emplace_back(start, std::move(_curves), bounds, closed);
	}

	SvgBounds SvgPathGenerator::CalculateBounds(const SvgPoint& a, const SvgPoint& b, const SvgPoint& c,
			const SvgPoint& d)
	{
		// Start the bounding box by end points
		SvgBounds bounds(a, d);

		// Bezier curve fits inside the convex hull of it's control points.
		// If control points are inside the bounds, we're done.
		if (bounds.Contains(b) && bounds.Contains(c))
		{
			return bounds;
		}

		double minX = bounds.GetMinX();
		double minY = bounds.GetMinY();
		double maxX = bounds.GetMaxX();
		double maxY = bounds.GetMaxY();
		// Add bezier curve inflection points in X and Y.
		AddBezierPoints(a.GetX(), b.GetX(), c.GetX(), d.GetX(), minX, maxX);
		AddBezierPoints(a.GetY(), b.GetY(), c.GetY(), d.GetY(), minY, maxY);

		return { minX, minY, maxX, maxY};
	}

	void SvgPathGenerator::AddBezierPoints(const double aX, const double bX, const double cX, const double dX,
			double& boundMin, double& boundMax)
	{
		const auto a = -3.0 * aX + 9.0 * bX - 9.0 * cX + 3.0 * dX;
		const auto b = 6.0 * aX - 12.0 * bX + 6.0 * cX;
		const auto c = 3.0 * bX - 3.0 * aX;
		std::size_t count = 0;

		double roots[2];

		constexpr auto epsilon = std::numeric_limits<double>::epsilon();

		if (std::abs(a) < epsilon)
		{
			if (std::abs(b) > epsilon)
			{
				const auto t = -c / b;
				if (t > epsilon && t < 1.0 - epsilon)
				{
					roots[count++] = t;
				}
			}
		}
		else
		{
			const auto b2ac = b * b - 4.0 * c * a;
			if (b2ac > epsilon)
			{
				auto t = (-b + std::sqrt(b2ac)) / (2.0 * a);
				if (t > epsilon && t < 1.0 - epsilon)
				{
					roots[count++] = t;
				}
				t = (-b - std::sqrt(b2ac)) / (2.0 * a);
				if (t > epsilon && t < 1.0 - epsilon)
				{
					roots[count++] = t;
				}
			}
		}
		for (std::size_t j = 0; j < count; j++)
		{
			const auto v = EvaluateBezier(roots[j], aX, bX, cX, dX);
			boundMin = std::min(boundMin, v);
			boundMax = std::max(boundMax, v);
		}
	}

	double SvgPathGenerator::EvaluateBezier(double t, double p0, double p1, double p2, double p3)
	{
		const double it = 1.0 - t;
		return it * it * it * p0 + 3.0 * it * it * t * p1 + 3.0 * it * t * t * p2 + t * t * t * p3;
	}
} // Elpida
