//
// Created by klapeto on 9/3/2024.
//

#include "Elpida/Svg/SvgViewPort.hpp"
#include "Elpida/Xml/XmlMap.hpp"

namespace Elpida
{

	SvgViewPort::SvgViewPort(const XmlMap& properties)
			:SvgViewPort()
	{
		_x = SvgLength(properties.GetValue("x"));
		_y = SvgLength(properties.GetValue("y"));

		auto& widthStr = properties.GetValue("width");
		if (!widthStr.empty())
		{
			_width = SvgLength(widthStr);
		}

		auto& heightStr = properties.GetValue("height");
		if (!heightStr.empty())
		{
			_height = SvgLength(heightStr);
		}
	}

	SvgViewPort::SvgViewPort()
			:_width(300.0, SvgUnits::Px), _height(150.0, SvgUnits::Px)
	{

	}
} // Elpida