//
// Created by klapeto on 28/12/2023.
//

#ifndef ELPIDA_SVG_SVGPOINT_HPP
#define ELPIDA_SVG_SVGPOINT_HPP

#include "SvgTransform.hpp"

namespace Elpida
{
	class SvgPoint
	{
	public:
		[[nodiscard]]
		double GetX() const
		{
			return _x;
		}

		[[nodiscard]]
		double GetY() const
		{
			return _y;
		}

		[[nodiscard]]
		double& GetRefX()
		{
			return _x;
		}

		[[nodiscard]]
		double& GetRefY()
		{
			return _y;
		}

		void ApplyTransform(const SvgTransform& transform)
		{
			transform.ApplyToPoint(_x, _y, _x, _y);
		}

		SvgPoint():_x(0.0), _y(0.0){}
		SvgPoint(const double x, const double y)
			: _x(x),
			  _y(y)
		{
		}

	private:
		double _x;
		double _y;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGPOINT_HPP
