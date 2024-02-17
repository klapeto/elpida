//
// Created by klapeto on 28/12/2023.
//

#ifndef ELPIDA_SVG_SVGPOINT_HPP
#define ELPIDA_SVG_SVGPOINT_HPP

#include "SvgTransform.hpp"

namespace Elpida
{
	class SvgPoint
	{
	public:
		[[nodiscard]]
		double GetX() const
		{
			return _x;
		}

		[[nodiscard]]
		double GetY() const
		{
			return _y;
		}

		[[nodiscard]]
		double& GetRefX()
		{
			return _x;
		}

		[[nodiscard]]
		double& GetRefY()
		{
			return _y;
		}

		void operator+=(const SvgPoint& other)
		{
			_x += other._x;
			_y += other._y;
		}

		SvgPoint operator*(const double value) const
		{
			return {_x * value, _y * value};
		}

		SvgPoint operator+(const SvgPoint& other) const
		{
			return {_x + other._x, _y + other._y};
		}

		SvgPoint operator-(const SvgPoint& other) const
		{
			return {_x - other._x, _y - other._y};
		}

		SvgPoint& operator*=(const double value)
		{
			_x *= value;
			_y *= value;
			return *this;
		}

		SvgPoint& operator/=(const double value)
		{
			_x /= value;
			_y /= value;
			return *this;
		}

		SvgPoint operator-() const
		{
			return {-_x, -_y};
		}

		void ApplyTransform(const SvgTransform& transform)
		{
			transform.ApplyToPoint2(_x, _y);
		}

		void Transform(const SvgTransform& transform)
		{
			transform.ApplyToPoint(_x, _y, _x, _y);
		}

		double Product() const
		{
			return _x * _x + _y * _y;
		}

		double Length() const
		{
			return sqrt(Product());
		}

		SvgPoint GetInverse() const
		{
			return {_y, -_x};
		}

		double Normalize()
		{
			const double length = Length();
			if (length > 1e-6)
			{
				const double id = 1.0 / length;
				_x *= id;
				_y *= id;
			}
			return length;
		}

		bool operator==(const SvgPoint& other) const
		{
			constexpr double tolerance = 0.01;
			const double dx = _x - other._x;
			const double dy = _y - other._y;
			return dx * dx + dy * dy < tolerance * tolerance;
		}

		[[nodiscard]]
		double GetDistance(const SvgPoint& other) const
		{
			const double dx = _x - other._x;
			const double dy = _y - other._y;
			return sqrt(dx * dx + dy * dy);
		}

		[[nodiscard]]
		double GetDirection(const SvgPoint& to) const
		{
			const double dx = to._x - _x;
			const double dy = to._y - _y;
			return dy / dx;
		}

		SvgPoint(): _x(0.0), _y(0.0)
		{
		}

		SvgPoint(const double x, const double y)
			: _x(x),
			  _y(y)
		{
		}

	private:
		double _x;
		double _y;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGPOINT_HPP
