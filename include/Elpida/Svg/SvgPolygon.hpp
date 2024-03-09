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

	class SvgTransform;

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

		void Transform(const SvgTransform& transform);

		explicit SvgPolygon(std::vector<SvgEdge>&& edges)
				:_edges(std::move(edges))
		{
			CalculateBounds();
		}

	private:
		std::vector<SvgEdge> _edges;
		SvgBounds _bounds;

		void CalculateBounds();
	};
} // Elpida

#endif //ELPIDA_SVG_SVGPOLYGON_HPP
