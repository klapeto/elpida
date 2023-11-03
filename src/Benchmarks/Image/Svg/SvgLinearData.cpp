//
// Created by klapeto on 3/11/2023.
//

#include "SvgLinearData.hpp"
#include "SvgUnits.hpp"

namespace Elpida
{
	SvgLinearData::SvgLinearData()
		: _x1(0.0f, SvgUnits::PERCENT),
		  _y1(0.0f, SvgUnits::PERCENT),
		  _x2(100.0f, SvgUnits::PERCENT),
		  _y2(0.0f, SvgUnits::PERCENT)
	{

	}
} // Elpida