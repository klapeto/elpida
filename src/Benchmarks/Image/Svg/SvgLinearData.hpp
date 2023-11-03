//
// Created by klapeto on 3/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGLINEARDATA_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGLINEARDATA_HPP

#include "SvgCoordinate.hpp"
namespace Elpida
{

	class SvgLinearData
	{
	public:
		const SvgCoordinate& GetX1() const
		{
			return _x1;
		}
		const SvgCoordinate& GetY1() const
		{
			return _y1;
		}
		const SvgCoordinate& GetX2() const
		{
			return _x2;
		}
		const SvgCoordinate& GetY2() const
		{
			return _y2;
		}

		SvgLinearData();
	private:
		SvgCoordinate _x1;
		SvgCoordinate _y1;
		SvgCoordinate _x2;
		SvgCoordinate _y2;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGLINEARDATA_HPP
