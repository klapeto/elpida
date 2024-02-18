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

		void ApplyTransform(const SvgTransform& transform)
		{
			_startControl.Transform(transform);
			_endControl.Transform(transform);
			_end.Transform(transform);
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
