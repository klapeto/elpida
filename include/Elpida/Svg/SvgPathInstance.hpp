//
// Created by klapeto on 3/1/2024.
//

#ifndef ELPIDA_SVG_SVGPATHINSTANCE_HPP
#define ELPIDA_SVG_SVGPATHINSTANCE_HPP

#include <vector>

#include "SvgBounds.hpp"
#include "SvgPoint.hpp"
#include "SvgCubicBezierCurve.hpp"

namespace Elpida {

	class SvgPathInstance
	{
	public:
		[[nodiscard]]
		const std::vector<SvgCubicBezierCurve>& GetCurves() const
		{
			return _curves;
		}

		[[nodiscard]]
		const SvgBounds& GetBounds() const
		{
			return _bounds;
		}

		[[nodiscard]]
		bool IsClosed() const
		{
			return _closed;
		}

		[[nodiscard]]
		const SvgPoint& GetStartPoint() const
		{
			return _startPoint;
		}

		SvgPathInstance(const SvgPoint& startPoint, std::vector<SvgCubicBezierCurve>&& curves, const SvgBounds& bounds,
		                const bool closed)
			: _startPoint(startPoint),
			  _curves(std::move(curves)),
			  _bounds(bounds),
			  _closed(closed)
		{
		}

	private:
		SvgPoint _startPoint;
		std::vector<SvgCubicBezierCurve> _curves;
		SvgBounds _bounds;
		bool _closed;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGPATHINSTANCE_HPP
