//
// Created by klapeto on 11/12/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGVIEWBOX_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGVIEWBOX_HPP

#include "SvgCoordinate.hpp"
namespace Elpida
{

	class SvgViewBox
	{
	public:
		double GetMinX() const
		{
			return _minX;
		}
		double GetMinY() const
		{
			return _minY;
		}
		double GetWidth() const
		{
			return _width;
		}
		double GetHeight() const
		{
			return _height;
		}

		SvgViewBox() = default;
		SvgViewBox(double minX, double minY, double width, double height)
			: _minX(minX), _minY(minY), _width(width), _height(height)
		{
		}
	private:
		double _minX;
		double _minY;
		double _width;
		double _height;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGVIEWBOX_HPP
