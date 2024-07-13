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
		static constexpr double Kappa = 0.5522847493;	// Length proportional to radius of a cubic bezier handle for 90deg arcs.

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

		void LineTo(const SvgPoint& start, const SvgPoint& end);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGPATHPARSER_HPP
