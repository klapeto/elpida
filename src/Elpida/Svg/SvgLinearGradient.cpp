//
// Created by klapeto on 21/12/2023.
//

#include "Elpida/Svg/SvgLinearGradient.hpp"

namespace Elpida
{
	SvgLinearGradient::SvgLinearGradient(const XmlElement& element, SvgDefs& defs)
		: SvgGradient(element, defs), _x1(0.0, SvgUnits::Percent), _y1(0.0, SvgUnits::Percent), _x2(100.0, SvgUnits::Percent),
		  _y2(100.0, SvgUnits::Percent)
	{
		ConditionallyAssignProperty("x1", _x1);
		ConditionallyAssignProperty("y1", _y1);
		ConditionallyAssignProperty("x2", _x2);
		ConditionallyAssignProperty("y2", _y2);
	}
} // Elpida
