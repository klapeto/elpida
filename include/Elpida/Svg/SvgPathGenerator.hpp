//
// Created by klapeto on 23/1/2024.
//

#ifndef ELPIDA_SVG_SVGPATHPARSER_HPP
#define ELPIDA_SVG_SVGPATHPARSER_HPP

#include <vector>

#include "SvgPathInstance.hpp"
#include "SvgCubicBezierCurve.hpp"

namespace Elpida
{
	class SvgPathGenerator final
	{
	public:
		std::vector<SvgPathInstance>& GetPaths()
		{
			return _paths;
		}

		void Reset()
		{
			_paths.clear();
			_curves.clear();
		}

		void ParsePathData(const std::string& data);

		void MoveTo(const SvgPoint& point, bool relative = false);
		void LineTo(const SvgPoint& point, bool relative = false);
		void HorizontalLineTo(double x, bool relative = false);
		void VerticalLineTo(double y, bool relative = false);
		void CubicBezTo(SvgPoint controlPointA, SvgPoint controlPointB, SvgPoint endPoint, bool relative = false);
		void CubicBezShortTo(SvgPoint controlPointB, SvgPoint endPoint, bool relative);
		void QuadBezTo(SvgPoint controlPoint, SvgPoint endPoint, bool relative);
		void QuadBezShortTo(SvgPoint endPoint, bool relative);
		void ArcTo(double radiusX,double radiusY,double xAxisRotation,bool largeArc,bool sweep,SvgPoint endpoint, bool relative);

		void CommitPath(bool closed);

		SvgPathGenerator()
			: _closed(false)
		{

		}
		~SvgPathGenerator() = default;
	private:
		std::vector<SvgPathInstance> _paths;
		std::vector<SvgCubicBezierCurve> _curves;
		SvgPoint _moveToPoint;
		SvgPoint _currentPoint;
		SvgPoint _previousControlPointB;
		bool _closed;

		static SvgBounds CalculateBounds(const SvgPoint& a, const SvgPoint& b, const SvgPoint& c, const SvgPoint& d);
		static void AddBezierPoints(double aX, double bX, double cX, double dX, double& boundMin,double& boundMax);
		static double EvaluateBezier(double t, double p0, double p1, double p2, double p3);
		void LineTo(const SvgPoint& start, const SvgPoint& end);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGPATHPARSER_HPP