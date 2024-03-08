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

		void Expand(const double rX, const double rY, double angle)
		{
			// the angle of the top point is the angle of the right point + 90 degrees
			_topPoint += SvgPoint(rY * cos(angle + std::numbers::pi / 2.0), rY * sin(angle + std::numbers::pi / 2.0));
			_rightPoint -= SvgPoint(rX * cos(angle), rX * sin(angle));
			Recalculate();
		}

		[[nodiscard]]
		double GetAngle() const
		{
			return _angle;
		}

		[[nodiscard]]
		double GetA() const
		{
			return _a;
		}

		[[nodiscard]]
		double GetB() const
		{
			return _b;
		}

		[[nodiscard]]
		SvgPoint CalculateClosestPoint(const SvgPoint& point) const
		{
			// See http://quickcalcbasic.com/ellipse%20line%20intersection.pdf

			if (_a == 0 || _b == 0)
			{
				return _center;
			}

			SvgLinearEquation equation(_center, point);

			auto n2 = _b * _b;
			auto h2 = _a * _a;

			auto cosA = _cosA;
			auto sinA = _sinA;

			auto cosA2 = cosA * cosA;
			auto sinA2 = sinA * sinA;

			if (equation.GetDirection().GetX() == 0)
			{
				return CalculateClosestPointVertical(point, n2, h2, cosA, sinA, cosA2, sinA2);
			}

			if (equation.GetDirection().GetY() == 0)
			{
				return CalculateClosestPointHorizontal(point, n2, h2, cosA, sinA, cosA2, sinA2);
			}

			auto m = equation.GetA();
			auto m2 = m * m;

			// translafe b1 to the off centered ellipse
			auto b1 = equation.GetB() + equation.GetA() * _center.GetX() - _center.GetY();
			auto b12 = b1 * b1;

			auto a = n2 * (cosA2 + (2 * m * cosA * sinA) + m2 * sinA2) + h2 * (m2 * cosA2 - (2 * m * cosA * sinA) +
				sinA2);
			auto b = (2 * n2 * b1) * (cosA * sinA + m * sinA2) + 2 * h2 * b1 * (m * cosA2 - cosA * sinA);
			auto c = b12 * (n2 * sinA2 + h2 * cosA2) - (h2 * n2);

			// Is directly at the elipsis
			if (a == 0.0)
			{
				return point;
			}

			double x1, x2;
			CalculateRoots(a, b, c, x1, x2);

			// revert translating
			x1 += _center.GetX();
			x2 += _center.GetX();
			auto y1 = equation.CalculateY(x1);
			auto y2 = equation.CalculateY(x2);

			return std::abs(point.GetX() - x1) > std::abs(point.GetX() - x2) ? SvgPoint(x2, y2) : SvgPoint(x1, y1);
		}

		void Transform(const SvgTransform& transform)
		{
			_topPoint.Transform(transform);
			_rightPoint.Transform(transform);
			_center.Transform(transform);

			Recalculate();
		}

		[[nodiscard]]
		bool IsPointInside(const SvgPoint& point) const
		{
			return CalculateNormalDistance(point) <= 1.0;
		}

		[[nodiscard]]
		double CalculateNormalDistance(const SvgPoint& point) const
		{
			if (_b == 0.0 || _a == 0.0) return point.GetDistance(_center);
			auto delta = point - _center;

			auto d1 = delta.GetX() * _cosA + delta.GetY() * _sinA;
			d1 = d1 * d1;

			auto d2 = delta.GetX() * _sinA - delta.GetY() * _cosA;
			d2 = d2 * d2;

			return (d1 / (_a * _a)) + (d2 / (_b * _b));
		}

		SvgEllipseEquation(const double cX, const double cY, const double rX, const double rY)
		{
			_center = SvgPoint(cX, cY);
			_rightPoint = SvgPoint(cX + rX, cY);
			_topPoint = SvgPoint(cX, cY - rY);
			Recalculate();
		}

		SvgEllipseEquation(const SvgPoint& center, const SvgPoint& top, const SvgPoint& right)
		{
			_center = center;
			_rightPoint = right;
			_topPoint = top;
			Recalculate();
		}

	private:
		SvgPoint _center;
		SvgPoint _rightPoint;
		SvgPoint _topPoint;
		double _angle;
		double _a;
		double _b;
		double _sinA;
		double _cosA;

		[[nodiscard]]
		SvgPoint CalculateClosestPointVertical(const SvgPoint& point, double n2, double h2, double cosA, double sinA,
		                                       double cosA2, double sinA2) const
		{
			auto x = point.GetX() - _center.GetX();

			auto a = n2 * sinA2 + h2 * cosA2;
			auto b = 2 * x * cosA * sinA * (n2 - h2);
			auto c = x * x * (n2 * cosA2 + h2 * sinA2) - h2 * n2;

			double y1, y2;
			CalculateRoots(a, b, c, y1, y2);

			// revert translating
			y1 += _center.GetY();
			y2 += _center.GetY();

			return std::abs(point.GetY() - y1) > std::abs(point.GetY() - y2)
				       ? SvgPoint(point.GetX(), y2)
				       : SvgPoint(point.GetX(), y1);
		}

		[[nodiscard]]
		SvgPoint CalculateClosestPointHorizontal(const SvgPoint& point, double n2, double h2, double cosA, double sinA,
		                                         double cosA2, double sinA2) const
		{
			auto y = point.GetY() - _center.GetY();
			auto a = n2 * cosA2 + h2 * sinA2;
			auto b = 2 * y * cosA * sinA * (n2 - h2);
			auto c = y * y * (n2 * sinA2 + h2 * cosA2) - h2 * n2;

			double x1, x2;
			CalculateRoots(a, b, c, x1, x2);

			x1 += _center.GetY();
			x2 += _center.GetY();

			return std::abs(point.GetX() - x1) > std::abs(point.GetX() - x2)
				       ? SvgPoint(x2, point.GetY())
				       : SvgPoint(x1, point.GetY());
		}

		static void CalculateRoots(const double a, const double b, const double c, double& r1, double& r2)
		{
			if (a == 0.0)
			{
				// first class?
				if (b == 0.0)
				{
					// not even first class
					r1 = c;
					r2 = c;
					return;
				}
				r1 = -c / b;
				r2 = r1;
				return;
			}
			auto delta = b * b - 4 * a * c;

			auto sqr = sqrt(delta);
			r1 = (-b + sqr) / (2 * a);
			r2 = (-b - sqr) / (2 * a);
		}

		void Recalculate()
		{
			_a = _center.GetDistance(_rightPoint);
			_b = _center.GetDistance(_topPoint);

			auto delta = _center - _rightPoint;
			_angle = atan2(delta.GetY(), delta.GetX());
			_sinA = std::sin(_angle);
			_cosA = std::cos(_angle);
		}
	};
} // Elpida

#endif //SVGELLIPSE_HPP
