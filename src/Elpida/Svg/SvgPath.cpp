//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgPath.hpp"

#include <Elpida/Svg/SvgNumber.hpp>
#include <Elpida/Svg/Shapes/SvgCubicBezierCurve.hpp>
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

	void SvgPath::MoveTo(
		SvgPoint& startPoint,
		const double a, const double b,
		const bool relative)
	{
		if (relative)
		{
			startPoint += SvgPoint(a, b);
		}
		else
		{
			startPoint = SvgPoint(a, b);
		}
	}


	void SvgPath::LineTo(const SvgPoint& startPoint, const SvgPoint& point, std::vector<SvgCubicBezierCurve>& curves)
	{
		const double px = startPoint.GetX();
		const double py = startPoint.GetY();
		const double x = point.GetX();
		const double y = point.GetY();
		const double dx = x - px;
		const double dy = y - py;
		curves.emplace_back(SvgPoint(px + dx / 3.0, py + dy / 3.0),
		                    SvgPoint(x - dx / 3.0, y - dy / 3.0),
		                    point);
	}

	void SvgPath::LineTo(
		SvgPoint& startPoint,
		const double a, const double b,
		const bool relative,
		std::vector<SvgCubicBezierCurve>& curves)
	{
		const auto initialStartPoint = startPoint;

		if (relative)
		{
			startPoint += SvgPoint(a, b);
		}
		else
		{
			startPoint = SvgPoint(a, b);
		}

		LineTo(initialStartPoint, startPoint, curves);
	}

	void SvgPath::CubicBezTo(const SvgPoint& controlPointA, const SvgPoint& controlPointB, const SvgPoint& end,
	                         std::vector<SvgCubicBezierCurve>& curves)
	{
		curves.emplace_back(controlPointA, controlPointB, end);
	}

	void SvgPath::LineTo(const SvgPoint& point, std::vector<SvgCubicBezierCurve>& curves)
	{
		if (!curves.empty())
		{
			LineTo(curves.back().GetEnd(), point, curves);
		}
	}

	void SvgPath::CubicBezTo(SvgPoint& startPoint,
	                         SvgPoint& previousControlPointB,
	                         const double a, const double b, const double c, const double d, const double e,
	                         const double f,
	                         const bool relative,
	                         std::vector<SvgCubicBezierCurve>& curves)
	{
		SvgPoint controlPointA(a, b);
		SvgPoint controlPointB(c, d);
		SvgPoint endPoint(e, f);

		if (relative)
		{
			controlPointA += startPoint;
			controlPointB += startPoint;
			endPoint += startPoint;
		}

		CubicBezTo(controlPointA, controlPointB, endPoint, curves);

		previousControlPointB = controlPointB;
		startPoint = endPoint;
	}

	void SvgPath::CubicBezShortTo(SvgPoint& startPoint, SvgPoint& previousControlPointB,
	                            const double a, const double b, const double c, const double d,
	                            const bool relative,
	                            std::vector<SvgCubicBezierCurve>& curves)
	{
		SvgPoint controlPointB(a,b);
		SvgPoint endPoint(c, d);

		if (relative)
		{
			controlPointB += startPoint;
			endPoint += startPoint;
		}

		const SvgPoint controlPointA(2 * startPoint.GetX() - controlPointB.GetX(), 2 * startPoint.GetY() - controlPointB.GetY());

		CubicBezTo(controlPointA, controlPointB, endPoint, curves);

		previousControlPointB = controlPointB;
		startPoint = endPoint;
	}

	void SvgPath::QuadBezTo(SvgPoint& startPoint, SvgPoint& previousControlPointB,
	                      const double a, const double b, const double c, const double d,
	                      const bool relative,
	                      std::vector<SvgCubicBezierCurve>& curves)
	{
		SvgPoint controlPoint(a, b);
		SvgPoint endPoint(c, d);

		if (relative)
		{
			controlPoint += startPoint;
			endPoint += startPoint;
		}

		// Convert to cubic bezier
		const SvgPoint controlPointA(
			startPoint.GetX() + 2.0 / 3.0 * (controlPoint.GetX() - startPoint.GetX()),
			startPoint.GetY() + 2.0 / 3.0 * (controlPoint.GetY() - startPoint.GetY()));


		const SvgPoint controlPointB(
			endPoint.GetX() + 2.0 / 3.0 * (controlPoint.GetX() - endPoint.GetX()),
			endPoint.GetY() + 2.0 / 3.0 * (controlPoint.GetY() - endPoint.GetY()));

		CubicBezTo(controlPointA, controlPointB, endPoint, curves);

		previousControlPointB = controlPointB;
		startPoint = endPoint;
	}


	void SvgPath::QuadBezShortTo(SvgPoint& startPoint, SvgPoint& previousControlPointB,
	                           const double a, const double b,
	                           const bool relative,
	                           std::vector<SvgCubicBezierCurve>& curves)
	{
		SvgPoint endPoint(a,b);

		if (relative)
		{
			endPoint += startPoint;
		}

		const double cx = 2 * startPoint.GetX() - previousControlPointB.GetX();
		const double cy = 2 * startPoint.GetY() - previousControlPointB.GetY();

		const SvgPoint controlPointA(
			startPoint.GetX() + 2.0 / 3.0 * (cx - startPoint.GetX()),
			startPoint.GetY() + 2.0 / 3.0 * (cy - startPoint.GetY())
		);

		const SvgPoint controlPointB(
			endPoint.GetX() + 2.0 / 3.0 * (cx - endPoint.GetX()),
			endPoint.GetY() + 2.0 / 3.0 * (cy - endPoint.GetY())
		);


		CubicBezTo(controlPointA, controlPointB, endPoint, curves);

		previousControlPointB = controlPointB;
		startPoint = endPoint;
	}

	void SvgPath::ArcTo(SvgPoint& startPoint,
	                  const double aA, const double aB, const double aC, const double aD, const double aE, const double aF,
	                  const double aG,
	                  const bool relative,
	                  std::vector<SvgCubicBezierCurve>& curves)
	{

		auto rx = fabs(aA); // y radius
		auto ry = fabs(aB); // x radius
		const auto rotx = aC / 180.0 * std::numbers::pi; // x rotation angle
		const auto fa = fabs(aD) > 1e-6; // Large arc
		const auto fs = fabs(aE) > 1e-6; // Sweep direction

		SvgPoint endpoint(aF, aG);

		if (relative)
		{
			endpoint += startPoint;
		}

		double dx = startPoint.GetX() - endpoint.GetX();
		double dy = startPoint.GetY() - endpoint.GetY();
		double distance = sqrt(dx * dx + dy * dy);
		if (distance < 1e-6 || rx < 1e-6 || ry < 1e-6)
		{
			// The arc degenerates to a line
			LineTo(startPoint, endpoint, curves);
			startPoint = endpoint;
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
		const double cx = (startPoint.GetX() + endpoint.GetX()) / 2.0 + cosrx * cxp - sinrx * cyp;
		const double cy = (startPoint.GetY() + endpoint.GetY()) / 2.0 + sinrx * cxp + cosrx * cyp;

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
				CubicBezTo(SvgPoint(px + ptanx, py + ptany), SvgPoint(x - tanx, y - tany), SvgPoint(x, y), curves);
			}

			px = x;
			py = y;
			ptanx = tanx;
			ptany = tany;
		}

		startPoint = endpoint;
	}

	void SvgPath::HorizontalLineTo(SvgPoint& startPoint,
	                             const double a,
	                             const bool relative,
	                             std::vector<SvgCubicBezierCurve>& curves)
	{
		const auto initialStartPoint = startPoint;
		if (relative)
		{
			startPoint.GetRefX() += a;
		}
		else
		{
			startPoint.GetRefX() = a;
		}
		LineTo(initialStartPoint, startPoint, curves);
	}

	void SvgPath::VerticalLineTo(SvgPoint& startPoint,
	                           const double a,
	                           const bool relative,
	                           std::vector<SvgCubicBezierCurve>& curves)
	{
		const auto initialStartPoint = startPoint;
		if (relative)
		{
			startPoint.GetRefY() += a;
		}
		else
		{
			startPoint.GetRefY() = a;
		}
		LineTo(initialStartPoint, startPoint, curves);
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

	SvgPath::SvgPath(const XmlElement& element, SvgDocument& document)
		: SvgShape(element, document)
	{
		auto& dData = GetProperties().GetValue("d");

		if (dData.empty()) return;
		CharacterStream stream(dData);

		try
		{
			std::vector<double> currentArguments;
			currentArguments.reserve(8);

			std::vector<SvgCubicBezierCurve> currentCurves;
			currentCurves.reserve(8);

			SvgPoint moveToPoint;
			SvgPoint startPoint;
			SvgPoint previousControlPointB;

			bool closed = false;
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
						CommitPath(moveToPoint, currentCurves, closed);
						closed = false;
						ParseAllNumbers(stream, currentArguments);
						if (currentArguments.empty() || currentArguments.size() % 2 != 0)
						{
							throw ParseException("moveTo command requires x,y");
						}

						MoveTo(moveToPoint, currentArguments[0], currentArguments[1], relative);

						startPoint = moveToPoint;
						for (std::size_t i = 2; i < currentArguments.size(); i += 2)
						{
							LineTo(startPoint, currentArguments[i], currentArguments[i + 1], relative, currentCurves);
						}
						previousControlPointB = startPoint;
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
							LineTo(startPoint, currentArguments[i], currentArguments[i + 1], relative, currentCurves);
						}
						previousControlPointB = startPoint;
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
							HorizontalLineTo(startPoint, currentArguments[i], relative, currentCurves);
						}
						previousControlPointB = startPoint;
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
							VerticalLineTo(startPoint, currentArguments[i], relative, currentCurves);
						}
						previousControlPointB = startPoint;
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
							CubicBezTo(startPoint, previousControlPointB,
							           currentArguments[i],
							           currentArguments[i + 1],
							           currentArguments[i + 2],
							           currentArguments[i + 3],
							           currentArguments[i + 4],
							           currentArguments[i + 5],
							           relative,
							           currentCurves);
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
							CubicBezShortTo(startPoint, previousControlPointB,
							                currentArguments[i],
							                currentArguments[i + 1],
							                currentArguments[i + 2],
							                currentArguments[i + 3],
							                relative,
							                currentCurves);
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
							QuadBezTo(startPoint, previousControlPointB,
							          currentArguments[i],
							          currentArguments[i + 1],
							          currentArguments[i + 2],
							          currentArguments[i + 3],
							          relative,
							          currentCurves);
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
							QuadBezShortTo(startPoint, previousControlPointB,
							               currentArguments[i],
							               currentArguments[i + 1],
							               relative,
							               currentCurves);
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
							ArcTo(startPoint,
							      currentArguments[i],
							      currentArguments[i + 1],
							      currentArguments[i + 2],
							      currentArguments[i + 3],
							      currentArguments[i + 4],
							      currentArguments[i + 5],
							      currentArguments[i + 6],
							      relative,
							      currentCurves);
						}
						previousControlPointB = startPoint;
					}
					break;
				case 'Z':
					[[fallthrough]];
				case 'z':
					closed = true;
					if (!currentCurves.empty())
					{
						startPoint = moveToPoint;
						previousControlPointB = startPoint;
						CommitPath(moveToPoint, currentCurves, closed);
					}
					closed = false;
					moveToPoint = startPoint;
					stream.Next();
					break;
				default:
					throw ParseException(c, "m,M,");
				}
			}
			CommitPath(moveToPoint, currentCurves, closed);
		}
		catch (const ParseException&)
		{
			_instances.clear();
		}
	}

	void SvgPath::CommitPath(const SvgPoint& moveToPoint, std::vector<SvgCubicBezierCurve>& curves, bool closed)
	{
		if (curves.empty())
		{
			return;
		}

		if (closed)
		{
			const auto& lastCurve = curves.back();
			LineTo(lastCurve.GetEnd(), moveToPoint, curves);
		}

		auto& transform = GetTransform();

		auto start = moveToPoint;
		start.ApplyTransform(transform);

		for (auto& curve : curves)
		{
			curve.ApplyTransform(transform);
		}

		const auto* currentCurve = &curves.front();

		SvgBounds bounds(start, currentCurve->GetStartControl(), currentCurve->GetEndControl(), currentCurve->GetEnd());
		for (std::size_t i = 1; i < curves.size() - 1; ++i)
		{
			auto& curve = curves[i];
			bounds.Merge(SvgBounds(currentCurve->GetEnd(), curve.GetStartControl(), curve.GetEndControl(), curve.GetEnd()));
			currentCurve = &curve;
		}

		_instances.emplace_back(start, std::move(curves), bounds, closed);
	}

} // Elpida
