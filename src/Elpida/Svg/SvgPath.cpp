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

	static void MoveTo(double x, double y, const bool relative)
	{
		if (relative)
		{
			cpx += x;
			cpy += y;
		}
		else
		{
			cpx = x;
			cpy = y;
		}
		LineTo(cpx, cpy);
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
			std::vector<double> currentNumbers;
			currentNumbers.reserve(8);

			double cpx = 0;
			double cpy = 0;
			double cpx2 = 0;
			double cpy2 = 0;

			auto moveTo = [&](auto x, auto y, const bool relative)
			{
				if (relative)
				{
					cpx += x;
					cpy += y;
				}
				else
				{
					cpx = x;
					cpy = y;
				}
				MoveTo(cpx, cpy);
			};

			auto lineTo = [&](const auto x, const auto y, const bool relative)
			{
				if (relative)
				{
					cpx += x;
					cpy += y;
				}
				else
				{
					cpx = x;
					cpy = y;
				}
				LineTo(cpx, cpy);
			};

			auto horizontalLineTo = [&](const auto x, const bool relative)
			{
				if (relative)
				{
					cpx += x;
				}
				else
				{
					cpx = x;
				}
				LineTo(cpx, cpy);
			};

			auto verticalLineTo = [&](const auto y, const bool relative)
			{
				if (relative)
				{
					cpy += y;
				}
				else
				{
					cpy = y;
				}
				LineTo(cpx, cpy);
			};

			auto cubicBeizerTo = [&](auto _x1, auto _y1, auto _x2, auto _y2, auto _x, auto _y, const bool relative)
			{
				double x2, y2, cx1, cy1, cx2, cy2;

				if (relative)
				{
					cx1 = cpx + _x1;
					cy1 = cpy + _y1;
					cx2 = cpx + _x2;
					cy2 = cpy + _y2;
					x2 = cpx + _x;
					y2 = cpy + _y;
				}
				else
				{
					cx1 = _x1;
					cy1 = _y1;
					cx2 = _x2;
					cy2 = _y2;
					x2 = _x;
					y2 = _y;
				}

				CubicBeizerTo(cx1, cy1, cx2, cy2, x2, y2);

				cpx2 = cx2;
				cpy2 = cy2;
				cpx = x2;
				cpy = y2;
			};

			auto smoothCubicBeizerTo = [&](auto _x2, auto _y2, auto _x, auto _y, const bool relative)
			{
				double x2, y2, cx2, cy2;

				const double x1 = cpx;
				const double y1 = cpy;
				if (relative)
				{
					cx2 = cpx + _x2;
					cy2 = cpy + _y2;
					x2 = cpx + _x;
					y2 = cpy + _y;
				}
				else
				{
					cx2 = _x2;
					cy2 = _y2;
					x2 = _x;
					y2 = _y;
				}

				const double cx1 = 2 * x1 - cpx2;
				const double cy1 = 2 * y1 - cpy2;

				CubicBeizerTo(cx1, cy1, cx2, cy2, x2, y2);

				cpx2 = cx2;
				cpy2 = cy2;
				cpx = x2;
				cpy = y2;
			};


			auto quadBeizerTo = [&](auto _x2, auto _y2, auto _x, auto _y, const bool relative)
			{
				double x1, y1, x2, y2, cx, cy;
				double cx1, cy1, cx2, cy2;

				x1 = cpx;
				y1 = cpy;
				if (relative)
				{
					cx = cpx + _x2;
					cy = cpy + _y2;
					x2 = cpx + _x;
					y2 = cpy + _y;
				}
				else
				{
					cx = _x2;
					cy = _y2;
					x2 = _x;
					y2 = _y;
				}

				cx1 = x1 + 2.0f / 3.0f * (cx - x1);
				cy1 = y1 + 2.0f / 3.0f * (cy - y1);
				cx2 = x2 + 2.0f / 3.0f * (cx - x2);
				cy2 = y2 + 2.0f / 3.0f * (cy - y2);

				CubicBeizerTo(cx1, cy1, cx2, cy2, x2, y2);

				cpx2 = cx;
				cpy2 = cy;
				cpx = x2;
				cpy = y2;
			};

			auto smoothQuadBeizerTo = [&](auto _x, auto _y, const bool relative)
			{
				float x1, y1, x2, y2, cx, cy;
				float cx1, cy1, cx2, cy2;

				x1 = cpx;
				y1 = cpy;
				if (relative)
				{
					x2 = cpx + _x;
					y2 = cpy + _y;
				}
				else
				{
					x2 = _x;
					y2 = _y;
				}

				cx = 2 * x1 - cpx2;
				cy = 2 * y1 - cpy2;

				// Convert to cubix bezier
				cx1 = x1 + 2.0f / 3.0f * (cx - x1);
				cy1 = y1 + 2.0f / 3.0f * (cy - y1);
				cx2 = x2 + 2.0f / 3.0f * (cx - x2);
				cy2 = y2 + 2.0f / 3.0f * (cy - y2);

				CubicBeizerTo(cx1, cy1, cx2, cy2, x2, y2);

				cpx2 = cx;
				cpy2 = cy;
				cpx = x2;
				cpy = y2;
			};

			auto arcTo = [&](auto _rx, auto _ry, auto _xRotation, bool _largeArc, bool _sweep, auto _x, auto _y,
			                 const bool relative)
			{
				// Ported from canvg (https://code.google.com/p/canvg/)
				double rx, ry, rotx;
				double x1, y1, x2, y2, cx, cy, dx, dy, d;
				double x1p, y1p, cxp, cyp, s, sa, sb;
				double ux, uy, vx, vy, a1, da;
				double x, y, tanx, tany, a, px = 0, py = 0, ptanx = 0, ptany = 0;
				double sinrx, cosrx;
				int i, ndivs;
				double hda, kappa;

				rx = fabsf(_rx); // y radius
				ry = fabsf(_ry); // x radius
				rotx = _xRotation / 180.0 * Pi; // x rotation angle
				x1 = cpx; // start point
				y1 = cpy;
				if (relative)
				{
					// end point
					x2 = cpx + _x;
					y2 = cpy + _y;
				}
				else
				{
					x2 = _x;
					y2 = _y;
				}

				dx = x1 - x2;
				dy = y1 - y2;
				d = sqrt(dx * dx + dy * dy);
				if (d < 1e-6 || rx < 1e-6 || ry < 1e-6)
				{
					// The arc degenerates to a line
					LineTo(x2, y2);
					cpx = x2;
					cpy = y2;
					return;
				}

				sinrx = sin(rotx);
				cosrx = cos(rotx);

				// Convert to center point parameterization.
				// http://www.w3.org/TR/SVG11/implnote.html#ArcImplementationNotes
				// 1) Compute x1', y1'
				x1p = cosrx * dx / 2.0 + sinrx * dy / 2.0;
				y1p = -sinrx * dx / 2.0 + cosrx * dy / 2.0;
				d = Square(x1p) / Square(rx) + Square(y1p) / Square(ry);
				if (d > 1)
				{
					d = sqrt(d);
					rx *= d;
					ry *= d;
				}
				// 2) Compute cx', cy'
				s = 0.0;
				sa = Square(rx) * Square(ry) - Square(rx) * Square(y1p) - Square(ry) * Square(x1p);
				sb = Square(rx) * Square(y1p) + Square(ry) * Square(x1p);
				if (sa < 0.0) sa = 0.0;
				if (sb > 0.0)
					s = sqrt(sa / sb);
				if (_largeArc == _sweep)
					s = -s;
				cxp = s * rx * y1p / ry;
				cyp = s * -ry * x1p / rx;

				// 3) Compute cx,cy from cx',cy'
				cx = (x1 + x2) / 2.0 + cosrx * cxp - sinrx * cyp;
				cy = (y1 + y2) / 2.0 + sinrx * cxp + cosrx * cyp;

				// 4) Calculate theta1, and delta theta.
				ux = (x1p - cxp) / rx;
				uy = (y1p - cyp) / ry;
				vx = (-x1p - cxp) / rx;
				vy = (-y1p - cyp) / ry;
				a1 = VectorAngle(1.0, 0.0, ux, uy); // Initial angle
				da = VectorAngle(ux, uy, vx, vy); // Delta angle

				//	if (vecrat(ux,uy,vx,vy) <= -1.0f) da = NSVG_PI;
				//	if (vecrat(ux,uy,vx,vy) >= 1.0f) da = 0;

				if (!_sweep && da > 0)
					da -= 2 * Pi;
				else if (_sweep && da < 0)
					da += 2 * Pi;

				// Approximate the arc using cubic spline segments.

				SvgTransform transform(cosrx, sinrx, -sinrx, cosrx, cx, cy);

				// Split arc into max 90 degree segments.
				// The loop assumes an iteration per end point (including start and end), this +1.
				ndivs = (int)(fabs(da) / (Pi * 0.5) + 1.0);
				hda = (da / (double)ndivs) / 2.0;
				// Fix for ticket #179: division by 0: avoid cotangens around 0 (infinite)
				if ((hda < 1e-3) && (hda > -1e-3))
					hda *= 0.5;
				else
					hda = (1.0 - cos(hda)) / sin(hda);
				kappa = fabs(4.0 / 3.0 * hda);
				if (da < 0.0)
					kappa = -kappa;

				for (i = 0; i <= ndivs; i++)
				{
					a = a1 + da * ((double)i / (double)ndivs);
					dx = cos(a);
					dy = sin(a);
					transform.ApplyToPoint(x,y, dx * rx, dy * ry);
					transform.ApplyToVector(tanx, tany, -dy * rx * kappa, dx * ry * kappa);
					if (i > 0)
						CubicBeizerTo( px + ptanx, py + ptany, x - tanx, y - tany, x, y);
					px = x;
					py = y;
					ptanx = tanx;
					ptany = tany;
				}

				cpx = x2;
				cpy = y2;
			};

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
						ParseAllNumbers(stream, currentNumbers);
						if (currentNumbers.empty() || currentNumbers.size() % 2 != 0)
						{
							throw ParseException("moveTo command requires x,y");
						}

						moveTo(currentNumbers[0], currentNumbers[1], relative);

						for (std::size_t i = 2; i < currentNumbers.size(); i += 2)
						{
							lineTo(currentNumbers[i], currentNumbers[i + 1], relative);
						}
					}

					break;
				case 'l':
					relative = true;
					[[fallthrough]];
				case 'L':
					{
						ParseAllNumbers(stream, currentNumbers);
						if (currentNumbers.empty() || currentNumbers.size() % 2 != 0)
						{
							throw ParseException("lineTo command requires x,y");
						}

						for (std::size_t i = 0; i < currentNumbers.size(); i += 2)
						{
							lineTo(currentNumbers[i], currentNumbers[i + 1], relative);
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
						ParseAllNumbers(stream, currentNumbers);

						if (currentNumbers.empty())
						{
							throw ParseException("horizontalLineTo command requires x");
						}

						for (std::size_t i = 0; i < currentNumbers.size(); i++)
						{
							horizontalLineTo(currentNumbers[i], relative);
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
						ParseAllNumbers(stream, currentNumbers);

						if (currentNumbers.empty())
						{
							throw ParseException("verticallLineTo command requires x");
						}

						for (std::size_t i = 0; i < currentNumbers.size(); i++)
						{
							verticalLineTo(currentNumbers[i], relative);
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
						ParseAllNumbers(stream, currentNumbers);

						if (currentNumbers.empty() || currentNumbers.size() % 6 != 0)
						{
							throw ParseException("curveTo command requires x1 y1 x2 y2 x y");
						}

						for (std::size_t i = 0; i < currentNumbers.size(); i += 6)
						{
							cubicBeizerTo(currentNumbers[i], currentNumbers[i + 1], currentNumbers[i + 2],
							              currentNumbers[i + 3], currentNumbers[i + 4], currentNumbers[i + 5],
							              relative);
						}
					}
					break;
				case 's':
					relative = false;
					[[fallthrough]];
				case 'S':
					{
						ParseAllNumbers(stream, currentNumbers);

						if (currentNumbers.empty() || currentNumbers.size() % 4 != 0)
						{
							throw ParseException("smoothCurveTo command requires x2 y2 x y");
						}

						for (std::size_t i = 0; i < currentNumbers.size(); i += 4)
						{
							smoothCubicBeizerTo(currentNumbers[i], currentNumbers[i + 1], currentNumbers[i + 2],
							                    currentNumbers[i + 3], relative);
						}
					}
					break;
				case 'q':
					relative = false;
					[[fallthrough]];
				case 'Q':
					{
						ParseAllNumbers(stream, currentNumbers);

						if (currentNumbers.empty() || currentNumbers.size() % 4 != 0)
						{
							throw ParseException("quadricCurveTo command requires x2 y2 x y");
						}

						for (std::size_t i = 0; i < currentNumbers.size(); i += 4)
						{
							quadBeizerTo(currentNumbers[i], currentNumbers[i + 1], currentNumbers[i + 2],
							             currentNumbers[i + 3], relative);
						}
					}
					break;
				case 't':
					relative = false;
					[[fallthrough]];
				case 'T':
					{
						ParseAllNumbers(stream, currentNumbers);
						if (currentNumbers.empty() || currentNumbers.size() % 2 != 0)
						{
							throw ParseException("smoothQuadricCurveTo command requires x y");
						}

						for (std::size_t i = 0; i < currentNumbers.size(); i += 2)
						{
							smoothQuadBeizerTo(currentNumbers[i], currentNumbers[i + 1], relative);
						}
					}
					break;
				case 'a':
					relative = false;
					[[fallthrough]];
				case 'A':
					{
						ParseAllNumbers(stream, currentNumbers);

						if (currentNumbers.empty() || currentNumbers.size() % 7 != 0)
						{
							throw ParseException(
								"arc command requires rx ry x-axis-rotation large-arc-flag sweep-flag x y");
						}

						for (std::size_t i = 0; i < currentNumbers.size(); i += 7)
						{
							arcTo(currentNumbers[i],
							      currentNumbers[i + 1],
							      currentNumbers[i + 2],
							      currentNumbers[i + 3] == 1,
							      currentNumbers[i + 4] == 1,
							      currentNumbers[i + 5],
							      currentNumbers[i + 6],
							      relative);
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
