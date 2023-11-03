//
// Created by klapeto on 3/11/2023.
//

#include "SvgRadialData.hpp"
#include "SvgUnits.hpp"

namespace Elpida
{
	SvgRadialData::SvgRadialData()
		: _cx(50.0f, SvgUnits::PERCENT),
		  _cy(50.0f, SvgUnits::PERCENT),
		  _r(50.0f, SvgUnits::PERCENT),
		  _fx(0.0f, SvgUnits::USER),
		  _fy(0.0f, SvgUnits::USER)
	{

	}
} // Elpida