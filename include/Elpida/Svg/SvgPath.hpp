//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGPATH_HPP
#define ELPIDA_SVG_SVGPATH_HPP

#include "SvgPathInstance.hpp"
#include "SvgPoint.hpp"
#include "SvgShape.hpp"
#include "Shapes/SvgCubicBezierCurve.hpp"

namespace Elpida
{
	class SvgPath : public SvgShape
	{
	public:
		[[nodiscard]]
		const std::vector<SvgPathInstance>& GetInstances() const
		{
			return _instances;
		}

		explicit SvgPath(const XmlElement& element, SvgDocument& document);
	protected:
		void CommitPath(const SvgPoint& moveToPoint, std::vector<SvgCubicBezierCurve>& curves, bool closed);
		static void LineTo(const SvgPoint& startPoint, const SvgPoint& point, std::vector<SvgCubicBezierCurve>& curves);
		static void CubicBezTo(const SvgPoint& controlPointA, const SvgPoint& controlPointB, const SvgPoint& end, std::vector<SvgCubicBezierCurve>& curves);

		static void LineTo(const SvgPoint& point, std::vector<SvgCubicBezierCurve>& curves);

		static constexpr double Kappa = 0.5522847493;
	private:
		std::vector<SvgPathInstance> _instances;

		static void MoveTo(SvgPoint& startPoint, double a, double b, bool relative);
		static void LineTo(SvgPoint& startPoint, double a, double b, bool relative, std::vector<SvgCubicBezierCurve>& curves);
		static void HorizontalLineTo(SvgPoint& startPoint, double a, bool relative, std::vector<SvgCubicBezierCurve>& curves);
		static void VerticalLineTo(SvgPoint& startPoint, double a, bool relative, std::vector<SvgCubicBezierCurve>& curves);
		static void CubicBezTo(SvgPoint& startPoint, SvgPoint& previousControlPointB, double a, double b, double c,
		                       double d, double e, double f, bool relative, std::vector<SvgCubicBezierCurve>& curves);
		static void CubicBezShortTo(SvgPoint& startPoint, SvgPoint& previousControlPointB, double a, double b, double c,
		                            double d, bool relative, std::vector<SvgCubicBezierCurve>& curves);
		static void QuadBezTo(SvgPoint& startPoint, SvgPoint& previousControlPointB, double a, double b, double c,
		                      double d, bool relative, std::vector<SvgCubicBezierCurve>& curves);
		static void QuadBezShortTo(SvgPoint& startPoint, SvgPoint& previousControlPointB, double a, double b,
		                           bool relative, std::vector<SvgCubicBezierCurve>& curves);
		static void ArcTo(SvgPoint& startPoint, double aA, double aB, double aC, double aD, double aE, double aF,
		                  double aG, bool relative, std::vector<SvgCubicBezierCurve>& curves);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGPATH_HPP
