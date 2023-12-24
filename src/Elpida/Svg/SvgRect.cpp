//
// Created by klapeto on 23/12/2023.
//

#include "Elpida/Svg/SvgRect.hpp"

#include <Elpida/Svg/SvgNumber.hpp>

namespace Elpida
{
	SvgRect::SvgRect(const XmlElement& element, SvgDocument& document)
		: SvgShape(element, document)
	{
		auto& properties = GetProperties();
		_x = SvgLength(properties.GetValue("x"));
		_y = SvgLength(properties.GetValue("y"));

		_width = SvgLength(properties.GetValue("width"));
		if (_width.GetValue() < 0.0)
		{
			_width = {};
		}

		_height = SvgLength(properties.GetValue("height"));
		if (_height.GetValue() < 0.0)
		{
			_height = {};
		}

		auto& rx = properties.GetValue("rx");
		auto& ry = properties.GetValue("ry");

		if (rx.empty() && ry.empty()) return;

		_rX = SvgLength(rx);
		_rY = SvgLength(ry);

		if (!rx.empty() && ry.empty())
		{
			_rY = _rX;
		}

		if (!ry.empty() && rx.empty())
		{
			_rX = _rY;
		}

		if (_rY.GetValue() < 0.0)
		{
			_rY = {};
		} else if (_rY.GetValue())

		if (_rX.GetValue() < 0.0)
		{
			_rX = {};
		}
	}
} // Elpida
