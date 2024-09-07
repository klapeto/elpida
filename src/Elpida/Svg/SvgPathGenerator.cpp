//
// Created by klapeto on 23/1/2024.
//

#include "Elpida/Svg/SvgPathGenerator.hpp"

#include <Elpida/Svg/SvgNumber.hpp>
#include <Elpida/Xml/CharacterStream.hpp>
#include <Elpida/Xml/ParseException.hpp>
#include <Elpida/Svg/SvgUtilities.hpp>

namespace Elpida
{
	static SvgFloat Square(const SvgFloat x)
	{
		return x * x;
	}

	static bool IsNumberic(const char c)
	{
		return SvgNumber::IsNumber(c) || c == '+' || c == '-' || c == '.';
	}

	static SvgFloat VectorMagnitude(const SvgFloat x, const SvgFloat y)
	{
		return std::sqrt(x * x + y * y);
	}

	static SvgFloat VectorCosTheta(const SvgFloat ux, const SvgFloat uy, const SvgFloat vx, const SvgFloat vy)
	{
		return (ux * vx + uy * vy) / (VectorMagnitude(ux, uy) * VectorMagnitude(vx, vy));
	}

	static SvgFloat VectorAngle(const SvgFloat ux, const SvgFloat uy, const SvgFloat vx, const SvgFloat vy)
	{
		SvgFloat res = VectorCosTheta(ux, uy, vx, vy);
		if (res < -1.0) res = -1.0;
		if (res > 1.0) res = 1.0;
		return ((ux * vy < uy * vx) ? SvgFloat(-1.0) : SvgFloat(1.0)) * std::acos(res);
	}

	static std::string_view GetNextNumberView(CharacterStream& stream)
	{
		stream.SkipSpace();
		std::size_t begin = stream.Index();
		std::size_t end = begin;
		bool dotGot = false;
		while (!stream.Eof())
		{
			auto c = stream.Current();
			if (SvgNumber::IsNumber(c))
			{
				stream.Next();
				continue;
			}
			else
			{
				if (c == 'e' || c == 'E')
				{
					if (!stream.Next())
					{
						throw ParseException("Invalid number: expected 'x.xxxe+x' format but there is EOF after 'e'");
					}
					auto sign = stream.Current();
					if (sign != '-' && sign != '+')
					{
						throw ParseException("Invalid number: expected 'x.xxxe+x' format but there no sign after 'e'");
					}
					if (!stream.Next())
					{
						throw ParseException("Invalid number: expected 'x.xxxe+x' format but there is EOF after 'e' sign");
					}

					if (!SvgNumber::IsNumber(stream.Current()))
					{
						throw ParseException("Invalid number: expected 'x.xxxe+x' format but there is no number after 'e' sign");
					}

					while (SvgNumber::IsNumber(stream.Current()) && stream.Next())
					{
						end = stream.Index();
					}
					if (!SvgNumber::IsNumber(stream.Current()))
					{
						end--;
					}
					break;
				}
				else
				{
					if (c == '.')
					{
						// numbers can be separated by next decimal eg: '1.25.05' is '1.25' and '.05'
						if (!dotGot)
						{
							dotGot = true;
							if (!stream.Next())
							{
								throw ParseException("Invalid number: expected 'x.x' format but there is EOF after '.'");
							}
							if (!SvgNumber::IsNumber(stream.Current()))
							{
								throw ParseException("Invalid number: expected 'x.x' format but there is no number after '.'");
							}
							continue;
						}
						else
						{
							end = stream.Index() - 1;
							break;
						}
					}

					// numbers can be separated by next sign eg: '1.25-5.23' is '1.25' and '-5.23'
					if (begin == stream.Index() && (c == '-' || c == '+'))
					{
						stream.Next();
						continue;
					}
					end = stream.Index() - 1;
					break;
				}
			}
		}

		if (stream.Eof())
		{
			end = stream.Index() - 1;
		}
		return stream.GetStringView(begin, end);
	}

	static void ParseAllNumbers(CharacterStream& stream, std::vector<SvgFloat>& currentNumbers)
	{
		currentNumbers.clear();
		stream.Next();
		while (!stream.Eof())
		{
			stream.SkipSpace();
			currentNumbers.push_back(SvgNumber::ParseNumber(GetNextNumberView(stream)));
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
			std::vector<SvgFloat> currentArguments;
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
								SvgPoint(currentArguments[i + 2], currentArguments[i + 3]),
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
								std::abs(currentArguments[i + 3]) > 1e-6,
								std::abs(currentArguments[i + 4]) > 1e-6,
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
				case 0:
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

	void SvgPathGenerator::CubicBezTo(SvgPoint controlPointA, SvgPoint controlPointB, SvgPoint endPoint,
			const bool relative)
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

	void SvgPathGenerator::CubicBezShortTo(SvgPoint controlPointB, SvgPoint endPoint, const bool relative)
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

	void SvgPathGenerator::QuadBezTo(SvgPoint controlPoint, SvgPoint endPoint, const bool relative)
	{
		if (relative)
		{
			controlPoint += _currentPoint;
			endPoint += _currentPoint;
		}

		// Convert to cubic bezier
		const SvgPoint controlPointA(
				_currentPoint.GetX() + SvgFloat(2.0) / SvgFloat(3.0) * (controlPoint.GetX() - _currentPoint.GetX()),
				_currentPoint.GetY() + SvgFloat(2.0) / SvgFloat(3.0) * (controlPoint.GetY() - _currentPoint.GetY()));

		const SvgPoint controlPointB(
				endPoint.GetX() + SvgFloat(2.0) / SvgFloat(3.0) * (controlPoint.GetX() - endPoint.GetX()),
				endPoint.GetY() + SvgFloat(2.0) / SvgFloat(3.0) * (controlPoint.GetY() - endPoint.GetY()));

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

		const SvgFloat cx = 2 * _currentPoint.GetX() - _previousControlPointB.GetX();
		const SvgFloat cy = 2 * _currentPoint.GetY() - _previousControlPointB.GetY();

		const SvgPoint controlPointA(
				_currentPoint.GetX() + SvgFloat(2.0) / SvgFloat(3.0) * (cx - _currentPoint.GetX()),
				_currentPoint.GetY() + SvgFloat(2.0) / SvgFloat(3.0) * (cy - _currentPoint.GetY())
		);

		const SvgPoint controlPointB(
				endPoint.GetX() + SvgFloat(2.0) / SvgFloat(3.0) * (cx - endPoint.GetX()),
				endPoint.GetY() + SvgFloat(2.0) / SvgFloat(3.0) * (cy - endPoint.GetY())
		);

		_curves.emplace_back(controlPointA, controlPointB, endPoint);

		_previousControlPointB = controlPointB;
		_currentPoint = endPoint;
	}

	void SvgPathGenerator::ArcTo(
			const SvgFloat radiusX,
			const SvgFloat radiusY,
			const SvgFloat xAxisRotation,
			const bool largeArc,
			const bool sweep,
			SvgPoint endpoint,
			const bool relative)
	{
		auto rx = std::abs(radiusX); // y radius
		auto ry = std::abs(radiusY); // x radius
		const SvgFloat rotx = xAxisRotation / SvgFloat(180.0) * SvgUtilities::Pi; // x rotation angle
		const auto fa = largeArc; // Large arc
		const auto fs = sweep; // Sweep direction

		if (relative)
		{
			endpoint += _currentPoint;
		}

		SvgFloat dx = _currentPoint.GetX() - endpoint.GetX();
		SvgFloat dy = _currentPoint.GetY() - endpoint.GetY();
		SvgFloat distance = std::sqrt(dx * dx + dy * dy);
		if (distance < 1e-6 || rx < 1e-6 || ry < 1e-6)
		{
			// The arc degenerates to a line
			LineTo(_currentPoint, endpoint);
			_currentPoint = endpoint;
			_previousControlPointB = endpoint;
			return;
		}

		const auto sinrx = std::sin(rotx);
		const auto cosrx = std::cos(rotx);

		// Convert to center point parameterization.
		// http://www.w3.org/TR/SVG11/implnote.html#ArcImplementationNotes
		// 1) Compute x1', y1'
		const SvgFloat x1p = cosrx * dx / SvgFloat(2.0) + sinrx * dy / SvgFloat(2.0);
		const SvgFloat y1p = -sinrx * dx / SvgFloat(2.0) + cosrx * dy / SvgFloat(2.0);
		distance = Square(x1p) / Square(rx) + Square(y1p) / Square(ry);
		if (distance > 1)
		{
			distance = std::sqrt(distance);
			rx *= distance;
			ry *= distance;
		}

		// 2) Compute cx', cy'
		SvgFloat s = 0.0;
		SvgFloat sa = Square(rx) * Square(ry) - Square(rx) * Square(y1p) - Square(ry) * Square(x1p);
		const SvgFloat sb = Square(rx) * Square(y1p) + Square(ry) * Square(x1p);
		if (sa < 0.0)
		{
			sa = 0.0;
		}

		if (sb > 0.0)
		{
			s = std::sqrt(sa / sb);
		}

		if (fa == fs)
		{
			s = -s;
		}

		const SvgFloat cxp = s * rx * y1p / ry;
		const SvgFloat cyp = s * -ry * x1p / rx;

		// 3) Compute cx,cy from cx',cy'
		const SvgFloat cx = (_currentPoint.GetX() + endpoint.GetX()) / SvgFloat(2.0) + cosrx * cxp - sinrx * cyp;
		const SvgFloat cy = (_currentPoint.GetY() + endpoint.GetY()) / SvgFloat(2.0) + sinrx * cxp + cosrx * cyp;

		// 4) Calculate theta1, and delta theta.
		const SvgFloat ux = (x1p - cxp) / rx;
		const SvgFloat uy = (y1p - cyp) / ry;
		const SvgFloat vx = (-x1p - cxp) / rx;
		const SvgFloat vy = (-y1p - cyp) / ry;
		const SvgFloat a1 = VectorAngle(1.0, 0.0, ux, uy); // Initial angle
		SvgFloat da = VectorAngle(ux, uy, vx, vy); // Delta angle

		//	if (vecrat(ux,uy,vx,vy) <= -1.0f) da = NSVG_PI;
		//	if (vecrat(ux,uy,vx,vy) >= 1.0f) da = 0;

		if (fs == 0 && da > 0)
		{
			da -= 2 * SvgUtilities::Pi;
		}
		else if (fs == 1 && da < 0)
		{
			da += 2 * SvgUtilities::Pi;
		}

		const SvgTransform transform(cosrx, sinrx, -sinrx, cosrx, cx, cy);
		// Approximate the arc using cubic spline segments.

		// Split arc into max 90 degree segments.
		// The loop assumes an iteration per end point (including start and end), this +1.
		const auto ndivs = static_cast<std::size_t>(std::abs(da) / (SvgUtilities::Pi * 0.5) + 1.0);
		auto hda = (da / SvgFloat(ndivs)) / SvgFloat(2.0);

		// Fix for ticket #179: division by 0: avoid cotangens around 0 (infinite)
		if (hda < 1e-3 && hda > -1e-3)
		{
			hda *= 0.5;
		}
		else
		{
			hda = (SvgFloat(1.0) - std::cos(hda)) / std::sin(hda);
		}

		SvgFloat kappa = std::abs(SvgFloat(4.0) / SvgFloat(3.0) * hda);

		if (da < 0.0)
		{
			kappa = -kappa;
		}

		SvgFloat tanx;
		SvgFloat tany;
		SvgFloat x;
		SvgFloat y;
		SvgFloat px = 0;
		SvgFloat py = 0;
		SvgFloat ptanx = 0;
		SvgFloat ptany = 0;
		for (std::size_t i = 0; i <= ndivs; i++)
		{
			const SvgFloat a = a1 + da * (static_cast<SvgFloat>(i) / static_cast<SvgFloat>(ndivs));

			dx = std::cos(a);
			dy = std::sin(a);

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

	void SvgPathGenerator::HorizontalLineTo(const SvgFloat x, const bool relative)
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

	void SvgPathGenerator::VerticalLineTo(const SvgFloat a, const bool relative)
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
		const SvgFloat px = start.GetX();
		const SvgFloat py = start.GetY();
		const SvgFloat x = end.GetX();
		const SvgFloat y = end.GetY();
		const SvgFloat dx = x - px;
		const SvgFloat dy = y - py;
		_curves.emplace_back(SvgPoint(px + dx / SvgFloat(3.0), py + dy / SvgFloat(3.0)),
				SvgPoint(x - dx / SvgFloat(3.0), y - dy / SvgFloat(3.0)),
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

		auto start = _moveToPoint;

		_paths.emplace_back(start, std::move(_curves), closed);
	}
} // Elpida
