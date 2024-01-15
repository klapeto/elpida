//
// Created by klapeto on 9/1/2024.
//

#ifndef ELPIDA_SVG_SVGCUBICBEZIERCURVE_HPP
#define ELPIDA_SVG_SVGCUBICBEZIERCURVE_HPP

#include <Elpida/Svg/SvgPoint.hpp>

namespace Elpida
{
	class SvgCubicBezierCurve
	{
	public:
		[[nodiscard]]
		const SvgPoint& GetStartControl() const
		{
			return _startControl;
		}

		[[nodiscard]]
		const SvgPoint& GetEndControl() const
		{
			return _endControl;
		}

		[[nodiscard]]
		const SvgPoint& GetEnd() const
		{
			return _end;
		}

		[[nodiscard]]
		SvgPoint CalculatePoint(const SvgPoint& startPoint, const double t) const
		{
			const auto a = 1.0 - t;
			const auto a2 = a * a;
			const auto a3 = a * a * a;
			const auto t2 = t * t;
			const auto t3 = t * t * t;
			const double bx = a3 * startPoint.GetX()
				+ 3 * a2 * t * _startControl.GetX()
				+ 3 * a * t2 * _endControl.GetX()
				+ t3 * _end.GetX();
			const double by = a3 * startPoint.GetY()
				+ 3 * a2 * t * _startControl.GetY()
				+ 3 * a * t2 * _endControl.GetY()
				+ t3 * _end.GetY();

			return SvgPoint(bx, by);
		}

		void ApplyTransform(const SvgTransform& transform)
		{
			_startControl.ApplyTransform(transform);
			_endControl.ApplyTransform(transform);
			_end.ApplyTransform(transform);
		}

		SvgCubicBezierCurve(const SvgPoint& startControl, const SvgPoint& endControl, const SvgPoint& end)
			: _startControl(startControl),
			  _endControl(endControl),
			  _end(end)
		{
		}

		SvgCubicBezierCurve(const SvgCubicBezierCurve&) = default;
		SvgCubicBezierCurve(SvgCubicBezierCurve&&) noexcept = default;
		SvgCubicBezierCurve& operator=(const SvgCubicBezierCurve&) = default;
		SvgCubicBezierCurve& operator=(SvgCubicBezierCurve&&) noexcept = default;
	private:
		SvgPoint _startControl;
		SvgPoint _endControl;
		SvgPoint _end;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGCUBICBEZIERCURVE_HPP
