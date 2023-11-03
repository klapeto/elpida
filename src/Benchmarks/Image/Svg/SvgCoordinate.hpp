//
// Created by klapeto on 3/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGCOORDINATE_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGCOORDINATE_HPP

#include "SvgUnits.hpp"
namespace Elpida
{

	class SvgCoordinate
	{
	public:
		explicit SvgCoordinate(float value, SvgUnits units);
	private:
		float _value;
		SvgUnits _units;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGCOORDINATE_HPP
