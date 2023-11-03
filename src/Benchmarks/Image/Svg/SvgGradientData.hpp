//
// Created by klapeto on 3/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENTDATA_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENTDATA_HPP

#include "SvgAttributePair.hpp"
#include "SvgGradientStop.hpp"
#include "SvgLinearData.hpp"
#include "SvgPaintType.hpp"
#include "SvgRadialData.hpp"
#include "SvgSpreadType.hpp"
#include "SvgUnits.hpp"
#include "SvgGradientUnits.hpp"
#include "SvgTransform.hpp"
#include <string>
#include <vector>

namespace Elpida
{

	class SvgGradientData
	{
	public:
		SvgGradientData(SvgPaintType type, const std::vector<SvgAttributePair>& attributes);
	private:
		std::string _id;
		std::string _ref;
		std::vector<SvgGradientStop> _stops;
		union {
			SvgLinearData _linearData;
			SvgRadialData _radialData;
		};
		SvgPaintType _type;
		SvgSpreadType _spread;
		SvgGradientUnits _units;
		SvgTransform _transform;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENTDATA_HPP
