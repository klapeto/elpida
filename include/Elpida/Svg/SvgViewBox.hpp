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
		SvgFloat GetMinX() const
		{
			return _min.GetX();
		}

		[[nodiscard]]
		SvgFloat GetMinY() const
		{
			return _min.GetY();
		}

		[[nodiscard]]
		SvgFloat GetWidth() const
		{
			return _width;
		}

		[[nodiscard]]
		SvgFloat GetHeight() const
		{
			return _height;
		}

		[[nodiscard]]
		SvgFloat GetLength() const
		{
			return _length;
		}

		SvgViewBox();
		SvgViewBox(SvgFloat minX, SvgFloat minY, SvgFloat width, SvgFloat height);
		explicit SvgViewBox(std::string_view view);
	private:
		SvgPoint _min;
		SvgFloat _width;
		SvgFloat _height;
		SvgFloat _length;

		void CalculateLength();
	};

} // Elpida

#endif //ELPIDA_SVG_SVGVIEWBOX_HPP
