//
// Created by klapeto on 6/3/2024.
//

#ifndef ELPIDA_SVG_SVGPOLYGON_HPP
#define ELPIDA_SVG_SVGPOLYGON_HPP
#include <vector>

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
		bool IsPointInsideEvenOdd(const SvgPoint& point) const;

		[[nodiscard]]
		bool IsPointInsideNonZero(const SvgPoint& point) const;

		explicit SvgPolygon(std::vector<SvgEdge>&& edges)
			: _edges(std::move(edges))
		{

		}
	private:
		std::vector<SvgEdge> _edges;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGPOLYGON_HPP
