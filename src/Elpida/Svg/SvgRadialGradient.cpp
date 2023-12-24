//
// Created by klapeto on 21/12/2023.
//

#include "Elpida/Svg/SvgRadialGradient.hpp"

#include <Elpida/Xml/ParseException.hpp>

namespace Elpida
{
	SvgRadialGradient::SvgRadialGradient(const XmlElement& element, SvgDocument& document)
		: SvgGradient(element, document), _cx(50.0, SvgUnits::Percent), _cy(50.0, SvgUnits::Percent), _r(50.0, SvgUnits::Percent),
		  _fx(0.0, SvgUnits::Percent), _fy(0.0, SvgUnits::Percent)
	{
		ConditionallyAssignProperty("cx", _cx);
		ConditionallyAssignProperty("cy", _cy);
		ConditionallyAssignProperty("r", _r);

		if (_r.GetValue() < 0.0)
		{
			_r = SvgLength(-_r.GetValue(), _r.GetUnits());
			//throw ParseException("negative radial gradient radius", "positive radius");
		}

		{
			auto& value = GetProperties().GetValue("fx");
			if (!value.empty())
			{
				_fx = SvgLength(value);
			}
			else
			{
				_fx = _cx;
			}
		}

		{
			auto& value = GetProperties().GetValue("fy");
			if (!value.empty())
			{
				_fy = SvgLength(value);
			}
			else
			{
				_fy = _cy;
			}
		}
	}
} // Elpida
