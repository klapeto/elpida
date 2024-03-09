//
// Created by klapeto on 11/12/2023.
//

#ifndef ELPIDA_SVG_SVGVIEWBOX_HPP
#define ELPIDA_SVG_SVGVIEWBOX_HPP

#include <string_view>

#include "Elpida/Svg/SvgPoint.hpp"

namespace Elpida
{
	class SvgViewBox
	{
	public:

		[[nodiscard]]
		bool IsValid() const
		{
			return _width > 0.0 && _height > 0.0;
		}

		[[nodiscard]]
		const SvgPoint& GetMin() const
		{
			return _min;
		}

		[[nodiscard]]
		double GetMinX() const
		{
			return _min.GetX();
		}

		[[nodiscard]]
		double GetMinY() const
		{
			return _min.GetY();
		}

		[[nodiscard]]
		double GetWidth() const
		{
			return _width;
		}

		[[nodiscard]]
		double GetHeight() const
		{
			return _height;
		}

		[[nodiscard]]
		double GetLength() const
		{
			return _length;
		}

		SvgViewBox();
		SvgViewBox(double minX, double minY, double width, double height);
		explicit SvgViewBox(std::string_view view);
	private:
		SvgPoint _min;
		double _width;
		double _height;
		double _length;

		void CalculateLength();
	};

} // Elpida

#endif //ELPIDA_SVG_SVGVIEWBOX_HPP
