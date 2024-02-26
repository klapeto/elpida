//
// Created by klapeto on 18/2/2024.
//

#ifndef ELPIDA_SVG_SVGELLIPSE_HPP
#define ELPIDA_SVG_SVGELLIPSE_HPP

#include <optional>

#include "SvgLinearEquation.hpp"
#include "SvgPoint.hpp"

namespace Elpida
{
	class SvgEllipseEquation
	{
	public:

		[[nodiscard]]
		const SvgPoint& GetCenter() const
		{
			return _center;
		}

		[[nodiscard]]
		const SvgPoint& GetTopPoint() const
		{
			return _topPoint;
		}

		[[nodiscard]]
		const SvgPoint& GetRightPoint() const
		{
			return _rightPoint;
		}

		[[nodiscard]]
		bool CalculateIntersectionPoints(const SvgPoint& point, SvgPoint& intersectionA, SvgPoint& intersectionB) const
		{
			// See http://quickcalcbasic.com/ellipse%20line%20intersection.pdf
			SvgLinearEquation equation(_center, point);


			auto n2 = _b * _b;
			auto h2 = _a * _a;

			auto cosA = cos(_angle);
			auto sinA = sin(_angle);

			auto cosA2 = cosA * cosA;
			auto sinA2 = sinA * sinA;

			if (equation.GetDirection().GetX() == 0)
			{
				auto x = point.GetX() - _center.GetX();
				auto a = n2 * sinA2 + h2 * cosA2;
				auto b = 2 * x * cosA * sinA * (n2 - h2);
				auto c = x * x * (n2 * cosA2 + h2 * sinA2) - h2 * n2;

				if (a == 0.0)
				{
					return false;
				}

				// Solve the quadratic equation
				auto delta = b * b - 4 * a * c;

				// no real solutions
				if (delta < 0.0)
				{
					return false;
				}

				auto sqr = sqrt(delta);
				auto y1 = (-b + sqr) / (2 * a);
				auto y2 = (-b - sqr) / (2 * a);

				// revert translating
				y1 += _center.GetY();
				y2 += _center.GetY();

				intersectionA = SvgPoint(point.GetX(), y1);
				intersectionB = SvgPoint(point.GetX(), y2);

				return true;
			}

			auto m = equation.GetA();
			auto m2 = m*m;

			// translafe b1 to the off centered ellipse
			auto b1 = equation.GetB() + equation.GetA() * _center.GetX() - _center.GetY();
			auto b12 = b1 * b1;

			auto a = n2 * (cosA2 + (2 * m * cosA * sinA) + m2 * sinA2) + h2 * (m2 * cosA2 - (2 * m * cosA * sinA) + sinA2);
			auto b = (2 * n2 * b1) * (cosA * sinA + m * sinA2) + 2 * h2 * b1 * (m * cosA2 - cosA * sinA);
			auto c = b12 * (n2 * sinA2 + h2 * cosA2) - (h2 * n2);

			// no solutions
			if (a == 0.0)
			{
				return false;
			}

			// Solve the quadratic equation
			auto delta = b * b - 4 * a * c;

			// no real solutions
			if (delta < 0.0)
			{
				return false;
			}

			auto sqr = sqrt(delta);
			auto x1 = (-b + sqr) / (2 * a);
			auto x2 = (-b - sqr) / (2 * a);

			// revert translatin
			x1 += _center.GetX();
			x2 += _center.GetX();
			auto y1 = equation.CalculateY(x1);
			auto y2 = equation.CalculateY(x2);

			intersectionA = SvgPoint(x1, y1);
			intersectionB = SvgPoint(x2, y2);

			return true;
		}

		bool CalculateClosestPoint(const SvgPoint& point, SvgPoint& closestPoint) const
		{
			//if (!ca)
		}

		void Transform(const SvgTransform& transform)
		{
			_topPoint.Transform(transform);
			_rightPoint.Transform(transform);
			_center.Transform(transform);

			Recalculate();
		}

		[[nodiscard]]
		SvgPoint GetIntersectionWithLine(const SvgPoint& point) const
		{
			auto nominator = _a * _b;
			auto a2 = _a * _a;
			auto b2 = _b * _b;
			auto x = point.GetX() * cos(_angle);
			auto y = point.GetY() * sin(_angle);
			auto x2 = x * x;
			auto y2 = y*y;
			auto den = sqrt(a2*y2 + b2*x2);

			return {((nominator / den)*x) , ((nominator / den)*y)};
		}

		[[nodiscard]]
		bool IsPointInside(const SvgPoint& point) const
		{
			if (_b == 0.0 || _a == 0.0) return false;
			auto delta = point - _center;

			auto d1 = delta.GetX() * cos(_angle) + delta.GetY() * sin(_angle);
			d1 = d1 * d1;

			auto d2 = delta.GetX() * sin(_angle) - delta.GetY() * cos(_angle);
			d2 = d2 * d2;

			return (d1 / (_a * _a)) + (d2 / (_b * _b)) <= 1.0;
			// const auto distance = _focusPointA.GetDistance(point) + _focusPointB.GetDistance(point);
			//
			// return distance <= _constantDistance;
		}

		SvgEllipseEquation(const double cX, const double cY, const double rX, const double rY)
		{
			_center = SvgPoint(cX, cY);
			_rightPoint = SvgPoint(cX + rX, cY);
			_topPoint = SvgPoint(cX, cY - rY);
			Recalculate();
		}

	private:
		SvgPoint _center;
		SvgPoint _rightPoint;
		SvgPoint _topPoint;
		double _angle;
		double _a;
		double _b;

		void Recalculate()
		{
			_a = _center.GetDistance(_rightPoint);
			_b = _center.GetDistance(_topPoint);

			auto delta = _center - _rightPoint;
			_angle = atan2(delta.GetY(), delta.GetX());
		}
	};
} // Elpida

#endif //SVGELLIPSE_HPP
