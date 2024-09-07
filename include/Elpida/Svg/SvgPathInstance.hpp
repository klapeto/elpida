//
// Created by klapeto on 3/1/2024.
//

#ifndef ELPIDA_SVG_SVGPATHINSTANCE_HPP
#define ELPIDA_SVG_SVGPATHINSTANCE_HPP

#include <vector>

#include "Elpida/Svg/SvgBounds.hpp"
#include "Elpida/Svg/SvgPoint.hpp"
#include "Elpida/Svg/SvgCubicBezierCurve.hpp"
#include "Elpida/Svg/SvgConfig.hpp"

namespace Elpida
{

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

		void Transform(const SvgTransform& transform);

		SvgPathInstance(const SvgPoint& startPoint, std::vector<SvgCubicBezierCurve>&& curves, bool closed);
	private:
		SvgPoint _startPoint;
		std::vector<SvgCubicBezierCurve> _curves;
		SvgBounds _bounds;
		bool _closed;

		static SvgFloat EvaluateBezier(SvgFloat t, SvgFloat p0, SvgFloat p1, SvgFloat p2, SvgFloat p3);
		static void AddBezierPoints(SvgFloat aX, SvgFloat bX, SvgFloat cX, SvgFloat dX, SvgFloat& boundMin, SvgFloat& boundMax);
		static SvgBounds CalculateBounds(const SvgPoint& a, const SvgPoint& b, const SvgPoint& c, const SvgPoint& d);

		void ReCalculateBounds();
	};

} // Elpida

#endif //ELPIDA_SVG_SVGPATHINSTANCE_HPP
