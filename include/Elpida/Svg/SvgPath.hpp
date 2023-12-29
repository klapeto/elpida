//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGPATH_HPP
#define ELPIDA_SVG_SVGPATH_HPP

#include "SvgBounds.hpp"
#include "SvgPoint.hpp"
#include "SvgShape.hpp"

namespace Elpida
{
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
		void CommitPath(std::vector<SvgPoint>& points, bool closed);
		static void LineTo(double x, double y, std::vector<SvgPoint>& points);
		static void MoveTo(double x, double y, std::vector<SvgPoint>& points);
		static void CubicBezTo(double cpx1, double cpy1, double cpx2, double cpy2, double x, double y,
		                       std::vector<SvgPoint>& points);

		static constexpr double Kappa = 0.5522847493;
	private:
		std::vector<SvgPathInstance> _instances;
		static void MoveTo(double& cpx, double& cpy, double a, double b, bool relative, std::vector<SvgPoint>& points);
		static void LineTo(double& cpx, double& cpy, double a, double b, bool relative, std::vector<SvgPoint>& points);
		static void HorizontalLineTo(double& cpx, const double& cpy, double a, bool relative,
		                             std::vector<SvgPoint>& points);
		static void VerticalLineTo(const double& cpx, double& cpy, double a, bool relative,
		                           std::vector<SvgPoint>& points);
		static void CubicBezTo(double& cpx, double& cpy, double& cpx2, double& cpy2, double a, double b, double c,
		                       double d, double e, double f, bool relative, std::vector<SvgPoint>& points);
		static void CubicBezShortTo(double& cpx, double& cpy, double& cpx2, double& cpy2, double a, double b, double c,
		                            double d, bool relative, std::vector<SvgPoint>& points);
		static void QuadBezTo(double& cpx, double& cpy, double& cpx2, double& cpy2, double a, double b, double c,
		                      double d, bool relative, std::vector<SvgPoint>& points);
		static void QuadBezShortTo(double& cpx, double& cpy, double& cpx2, double& cpy2, double a, double b,
		                           bool relative, std::vector<SvgPoint>& points);
		static void ArcTo(double& cpx, double& cpy, double aA, double aB, double aC, double aD, double aE, double aF,
		                  double aG, bool relative, std::vector<SvgPoint>& points);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGPATH_HPP
