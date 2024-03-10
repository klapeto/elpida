//
// Created by klapeto on 3/1/2024.
//

#ifndef ELPIDA_SVG_SVGPATHINSTANCE_HPP
#define ELPIDA_SVG_SVGPATHINSTANCE_HPP

#include <vector>

#include "Elpida/Svg/SvgBounds.hpp"
#include "Elpida/Svg/SvgPoint.hpp"
#include "Elpida/Svg/SvgCubicBezierCurve.hpp"

namespace Elpida
{

	class SvgPathInstance
	{
	public:
		[[nodiscard]]
		const std::vector<SvgCubicBezierCurve>& GetCurves() const;

		[[nodiscard]]
		const SvgBounds& GetBounds() const;

		[[nodiscard]]
		bool IsClosed() const;

		[[nodiscard]]
		const SvgPoint& GetStartPoint() const;

		void Transform(const SvgTransform& transform);

		SvgPathInstance(const SvgPoint& startPoint, std::vector<SvgCubicBezierCurve>&& curves, bool closed);
	private:
		SvgPoint _startPoint;
		std::vector<SvgCubicBezierCurve> _curves;
		SvgBounds _bounds;
		bool _closed;

		static double EvaluateBezier(double t, double p0, double p1, double p2, double p3);
		static void AddBezierPoints(double aX, double bX, double cX, double dX, double& boundMin, double& boundMax);
		static SvgBounds CalculateBounds(const SvgPoint& a, const SvgPoint& b, const SvgPoint& c, const SvgPoint& d);

		void ReCalculateBounds();
	};

} // Elpida

#endif //ELPIDA_SVG_SVGPATHINSTANCE_HPP
