//
// Created by klapeto on 11/12/2023.
//

#ifndef ELPIDA_SVG_SVGVIEWBOX_HPP
#define ELPIDA_SVG_SVGVIEWBOX_HPP

#include <string_view>

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

		SvgViewBox(const double minX, const double minY, const double width, const double height)
			: _minX(minX),
			  _minY(minY),
			  _width(width),
			  _height(height)
		{
		}

		explicit SvgViewBox(std::string_view view);
	private:
		double _minX;
		double _minY;
		double _width;
		double _height;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGVIEWBOX_HPP
