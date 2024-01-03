//
// Created by klapeto on 3/1/2024.
//

#ifndef ELPIDA_SVG_SVGPATHINSTANCE_HPP
#define ELPIDA_SVG_SVGPATHINSTANCE_HPP

#include <vector>

#include "SvgBounds.hpp"
#include "SvgPoint.hpp"

namespace Elpida {

	class SvgPathInstance
	{
	public:
		[[nodiscard]]
		const std::vector<SvgPoint>& GetPoints() const
		{
			return _points;
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

		SvgPathInstance(std::vector<SvgPoint>&& points, const SvgBounds& bounds, const bool closed)
			: _points(std::move(points)),
			  _bounds(bounds),
			  _closed(closed)
		{
		}

	private:
		std::vector<SvgPoint> _points;
		SvgBounds _bounds;
		bool _closed;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGPATHINSTANCE_HPP
