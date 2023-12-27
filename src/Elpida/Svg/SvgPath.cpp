//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgPath.hpp"

#include <Elpida/Svg/SvgNumber.hpp>
#include <Elpida/Xml/CharacterStream.hpp>
#include <Elpida/Xml/ParseException.hpp>

namespace Elpida
{
	constexpr double Pi = 3.14159265358979323846264338327;

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
		if (res < -1.0f) res = -1.0f;
		if (res > 1.0f) res = 1.0f;
		return (x1 * y2 < y1 * x2 ? -1.0f : 1.0f) * acos(res);
	}

	static void MoveTo(const double x, const double y, std::vector<SvgPathPoint>& points)
	{
		if (!points.empty())
		{
			points.back() = SvgPathPoint(x, y);
		}
		else
		{
			points.emplace_back(x, y);
		}
	}

	static void MoveTo(double& cpx, double& cpy,
	                   const double a, const double b,
	                   const bool relative,
	                   std::vector<SvgPathPoint>& points)
	{
		if (relative)
		{
			cpx += a;
			cpy += b;
		}
		else
		{
			cpx = a;
			cpy = b;
		}
		MoveTo(cpx, cpy, points);
	}


	static void LineTo(const double x, const double y, std::vector<SvgPathPoint>& points)
	{
		if (!points.empty())
		{
			const auto& last = points.back();
			const double px = last.GetX();
			const double py = last.GetY();
			const double dx = x - px;
			const double dy = y - py;
			points.emplace_back(px + dx / 3.0f, py + dy / 3.0f);
			points.emplace_back(x - dx / 3.0f, y - dy / 3.0f);
			points.emplace_back(x, y);
		}
	}

	static void LineTo(
		double& cpx,
		double& cpy,
		const double a, const double b,
		const bool relative,
		std::vector<SvgPathPoint>& points)
	{
		if (relative)
		{
			cpx += a;
			cpy += b;
		}
		else
		{
			cpx = a;
			cpy = b;
		}
		LineTo(cpx, cpy, points);
	}

	static void CubicBezTo(double cpx1, double cpy1, double cpx2, double cpy2, double x, double y,
	                       std::vector<SvgPathPoint>& points)
	{
		if (!points.empty())
		{
			points.emplace_back(cpx1, cpy1);
			points.emplace_back(cpx2, cpy2);
			points.emplace_back(x, y);
		}
	}

	static void CubicBezTo(double& cpx, double& cpy,
	                       double& cpx2, double& cpy2,
	                       const double a, const double b, const double c, const double d, const double e,
	                       const double f,
	                       const bool relative,
	                       std::vector<SvgPathPoint>& points)
	{
		double x2, y2, cx1, cy1, cx2, cy2;

		if (relative)
		{
			cx1 = cpx + a;
			cy1 = cpy + b;
			cx2 = cpx + c;
			cy2 = cpy + d;
			x2 = cpx + e;
			y2 = cpy + f;
		}
		else
		{
			cx1 = a;
			cy1 = b;
			cx2 = c;
			cy2 = d;
			x2 = e;
			y2 = f;
		}

		CubicBezTo(cx1, cy1, cx2, cy2, x2, y2, points);

		cpx2 = cx2;
		cpy2 = cy2;
		cpx = x2;
		cpy = y2;
	}

	static void CubicBezShortTo(double& cpx, double& cpy,
	                            double& cpx2, double& cpy2,
	                            const double a, const double b, const double c, const double d,
	                            const bool relative,
	                            std::vector<SvgPathPoint>& points)
	{
		double x2, y2, cx2, cy2;

		const double x1 = cpx;
		const double y1 = cpy;

		if (relative)
		{
			cx2 = cpx + a;
			cy2 = cpy + b;
			x2 = cpx + c;
			y2 = cpy + d;
		}
		else
		{
			cx2 = a;
			cy2 = b;
			x2 = c;
			y2 = d;
		}

		const double cx1 = 2 * x1 - cpx2;
		const double cy1 = 2 * y1 - cpy2;

		CubicBezTo(cx1, cy1, cx2, cy2, x2, y2, points);

		cpx2 = cx2;
		cpy2 = cy2;
		cpx = x2;
		cpy = y2;
	}

	static void QuadBezTo(double& cpx, double& cpy,
	                      double& cpx2, double& cpy2,
	                      const double a, const double b, const double c, const double d,
	                      const bool relative,
	                      std::vector<SvgPathPoint>& points)
	{
		double x2, y2, cx, cy;

		const double x1 = cpx;
		const double y1 = cpy;

		if (relative)
		{
			cx = cpx + a;
			cy = cpy + b;
			x2 = cpx + c;
			y2 = cpy + d;
		}
		else
		{
			cx = a;
			cy = b;
			x2 = c;
			y2 = d;
		}

		// Convert to cubic bezier
		const double cx1 = x1 + 2.0f / 3.0f * (cx - x1);
		const double cy1 = y1 + 2.0f / 3.0f * (cy - y1);
		const double cx2 = x2 + 2.0f / 3.0f * (cx - x2);
		const double cy2 = y2 + 2.0f / 3.0f * (cy - y2);

		CubicBezTo(cx1, cy1, cx2, cy2, x2, y2, points);

		cpx2 = cx;
		cpy2 = cy;
		cpx = x2;
		cpy = y2;
	}


	static void QuadBezShortTo(double& cpx, double& cpy,
	                           double& cpx2, double& cpy2,
	                           const double a, const double b,
	                           const bool relative,
	                           std::vector<SvgPathPoint>& points)
	{
		double x2, y2;

		const double x1 = cpx;
		const double y1 = cpy;

		if (relative)
		{
			x2 = cpx + a;
			y2 = cpy + b;
		}
		else
		{
			x2 = a;
			y2 = b;
		}

		const double cx = 2 * x1 - cpx2;
		const double cy = 2 * y1 - cpy2;

		// Convert to cubix bezier
		const double cx1 = x1 + 2.0f / 3.0f * (cx - x1);
		const double cy1 = y1 + 2.0f / 3.0f * (cy - y1);
		const double cx2 = x2 + 2.0f / 3.0f * (cx - x2);
		const double cy2 = y2 + 2.0f / 3.0f * (cy - y2);

		CubicBezTo(cx1, cy1, cx2, cy2, x2, y2, points);

		cpx2 = cx;
		cpy2 = cy;
		cpx = x2;
		cpy = y2;
	}

	static void ArcTo(double& cpx, double& cpy,
	                  const double a, const double b, const double c, const double d, const double e, const double f,
	                  const double g,
	                  const bool relative,
	                  std::vector<SvgPathPoint>& points)
	{
		double x2;
		double y2;
		double x;
		double y;
		double px = 0;
		double py = 0;
		double ptanx = 0;
		double ptany = 0;

		auto rx = fabs(a); // y radius
		auto ry = fabs(b); // x radius
		const auto rotx = c / 180.0 * Pi; // x rotation angle
		const auto fa = fabs(d) > 1e-6; // Large arc
		const auto fs = fabs(e) > 1e-6; // Sweep direction
		const auto x1 = cpx; // start point
		const auto y1 = cpy;
		if (relative)
		{
			// end point
			x2 = cpx + f;
			y2 = cpy + g;
		}
		else
		{
			x2 = f;
			y2 = g;
		}

		double dx = x1 - x2;
		double dy = y1 - y2;
		double d = sqrt(dx * dx + dy * dy);
		if (d < 1e-6 || rx < 1e-6 || ry < 1e-6)
		{
			// The arc degenerates to a line
			LineTo(x2, y2, points);
			cpx = x2;
			cpy = y2;
			return;
		}

		const auto sinrx = sin(rotx);
		const auto cosrx = cos(rotx);

		// Convert to center point parameterization.
		// http://www.w3.org/TR/SVG11/implnote.html#ArcImplementationNotes
		// 1) Compute x1', y1'
		const double x1p = cosrx * dx / 2.0 + sinrx * dy / 2.0;
		const double y1p = -sinrx * dx / 2.0 + cosrx * dy / 2.0;
		d = Square(x1p) / Square(rx) + Square(y1p) / Square(ry);
		if (d > 1)
		{
			d = sqrt(d);
			rx *= d;
			ry *= d;
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
		const double cx = (x1 + x2) / 2.0 + cosrx * cxp - sinrx * cyp;
		const double cy = (y1 + y2) / 2.0 + sinrx * cxp + cosrx * cyp;

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
			da -= 2 * Pi;
		}
		else if (fs == 1 && da < 0)
		{
			da += 2 * Pi;
		}

		const SvgTransform transform(cosrx, sinrx, -sinrx, cosrx, cx, cy);
		// Approximate the arc using cubic spline segments.

		// Split arc into max 90 degree segments.
		// The loop assumes an iteration per end point (including start and end), this +1.
		const auto ndivs = static_cast<std::size_t>(fabs(da) / (Pi * 0.5) + 1.0);
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
		for (std::size_t i = 0; i <= ndivs; i++)
		{
			const double a = a1 + da * (static_cast<double>(i) / static_cast<double>(ndivs));

			dx = cos(a);
			dy = sin(a);

			transform.ApplyToPoint(x, y, dx * rx, dy * ry); // position
			transform.ApplyToVector(tanx, tany, -dy * rx * kappa, dx * ry * kappa); // tangent

			if (i > 0)
			{
				CubicBezTo(px + ptanx, py + ptany, x - tanx, y - tany, x, y, points);
			}

			px = x;
			py = y;
			ptanx = tanx;
			ptany = tany;
		}

		cpx = x2;
		cpy = y2;
	}

	static void HorizontalLineTo(double& cpx, const double& cpy,
	                             const double a,
	                             const bool relative,
	                             std::vector<SvgPathPoint>& points)
	{
		if (relative)
		{
			cpx += a;
		}
		else
		{
			cpx = a;
		}
		LineTo(cpx, cpy, points);
	}

	static void VerticalLineTo(const double& cpx, double& cpy,
	                           const double a,
	                           const bool relative,
	                           std::vector<SvgPathPoint>& points)
	{
		if (relative)
		{
			cpy += a;
		}
		else
		{
			cpy = a;
		}
		LineTo(cpx, cpy, points);
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

			std::vector<SvgPathPoint> currentPoints;
			currentPoints.reserve(8);

			double cpx = 0;
			double cpy = 0;
			double cpx2 = 0;
			double cpy2 = 0;

			while (!stream.Eof())
			{
				stream.SkipSpace();
				bool relative = false;
				switch (auto c = stream.Current())
				{
				case 'm':
					relative = true;
					[[fallthrough]];
				case 'M':
					{
						ParseAllNumbers(stream, currentArguments);
						if (currentArguments.empty() || currentArguments.size() % 2 != 0)
						{
							throw ParseException("moveTo command requires x,y");
						}

						MoveTo(cpx, cpy, currentArguments[0], currentArguments[1], relative, currentPoints);

						for (std::size_t i = 2; i < currentArguments.size(); i += 2)
						{
							LineTo(cpx, cpy, currentArguments[i], currentArguments[i + 1], relative, currentPoints);
						}
						cpx2 = cpx;
						cpy2 = cpy;
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
							LineTo(cpx, cpy, currentArguments[i], currentArguments[i + 1], relative, currentPoints);
						}
						cpx2 = cpx;
						cpy2 = cpy;
					}
					break;
				case 'h':
					relative = false;
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
							HorizontalLineTo(cpx, cpy, currentArguments[i], relative, currentPoints);
						}
						cpx2 = cpx;
						cpy2 = cpy;
					}
					break;
				case 'v':
					relative = false;
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
							VerticalLineTo(cpx, cpy, currentArguments[i], relative, currentPoints);
						}
						cpx2 = cpx;
						cpy2 = cpy;
					}
					break;
				case 'c':
					relative = false;
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
							CubicBezTo(cpx, cpy, cpx2, cpy2,
							           currentArguments[i],
							           currentArguments[i + 1],
							           currentArguments[i + 2],
							           currentArguments[i + 3],
							           currentArguments[i + 4],
							           currentArguments[i + 5],
							           relative,
							           currentPoints);
						}
					}
					break;
				case 's':
					relative = false;
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
							CubicBezShortTo(cpx, cpy, cpx2, cpy2,
							                currentArguments[i],
							                currentArguments[i + 1],
							                currentArguments[i + 2],
							                currentArguments[i + 3],
							                relative,
							                currentPoints);
						}
					}
					break;
				case 'q':
					relative = false;
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
							QuadBezTo(cpx, cpy, cpx2, cpy2,
							          currentArguments[i],
							          currentArguments[i + 1],
							          currentArguments[i + 2],
							          currentArguments[i + 3],
							          relative,
							          currentPoints);
						}
					}
					break;
				case 't':
					relative = false;
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
							QuadBezShortTo(cpx, cpy, cpx2, cpy2,
							               currentArguments[i],
							               currentArguments[i + 1],
							               relative,
							               currentPoints);
						}
					}
					break;
				case 'a':
					relative = false;
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
							ArcTo(cpx, cpy,
							      currentArguments[i],
							      currentArguments[i + 1],
							      currentArguments[i + 2],
							      currentArguments[i + 3],
							      currentArguments[i + 4],
							      currentArguments[i + 5],
							      currentArguments[i + 6],
							      relative,
							      currentPoints);
						}
						cpx2 = cpx;
						cpy2 = cpy;
					}
					break;
				case 'Z':
					[[fallthrough]];
				case 'z':
					_closed = true;
					stream.Next();
					break;
				default:
					throw ParseException(c, "m,M,");
				}
			}
		}
		catch (const ParseException&)
		{
			_points.clear();
		}
	}

	void SvgPath::AddPoint(double x, double y)
	{
		_points.emplace_back(x, y);
	}

	void SvgPath::MoveTo(const double x, const double y)
	{
		if (!_points.empty())
		{
			_points.back() = SvgPathPoint(x, y);
		}
		else
		{
			AddPoint(x, y);
		}
	}

	void SvgPath::LineTo(const double x, const double y)
	{
		if (!_points.empty())
		{
			const auto& last = _points.back();

			const auto dx = x - last.GetX();
			const auto dy = y - last.GetY();

			// We create a line with 4 points
			AddPoint(last.GetX() + dx / 3.0, last.GetY() + dy / 3.0);
			AddPoint(x - dx / 3.0, y - dy / 3.0);
			AddPoint(x, y);
		}
	}

	void SvgPath::CubicBeizerTo(const double x1,
	                            const double y1,
	                            const double x2,
	                            const double y2,
	                            const double x,
	                            const double y)
	{
		if (!_points.empty())
		{
			AddPoint(x1, y1);
			AddPoint(x2, y2);
			AddPoint(x, y);
		}
	}
} // Elpida
