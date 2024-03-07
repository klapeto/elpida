//
// Created by klapeto on 6/3/2024.
//

#ifndef ELPIDA_SVG_SVGPOLYGON_HPP
#define ELPIDA_SVG_SVGPOLYGON_HPP

#include <vector>

#include "SvgBounds.hpp"
#include "SvgEdge.hpp"

namespace Elpida
{

	class SvgPolygon
	{
	public:
		[[nodiscard]]
		const std::vector<SvgEdge>& GetEdges() const
		{
			return _edges;
		}

		[[nodiscard]]
		const SvgBounds& GetBounds() const
		{
			return _bounds;
		}

		[[nodiscard]]
		bool IsPointInsideEvenOdd(const SvgPoint& point) const;

		[[nodiscard]]
		bool IsPointInsideNonZero(const SvgPoint& point) const;

		explicit SvgPolygon(std::vector<SvgEdge>&& edges, const SvgBounds& bounds)
				:_edges(std::move(edges)), _bounds(bounds)
		{

		}

	private:
		std::vector<SvgEdge> _edges;
		SvgBounds _bounds;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGPOLYGON_HPP
