//
// Created by klapeto on 23/12/2023.
//

#ifndef ELPIDA_SVG_SVGRECT_HPP
#define ELPIDA_SVG_SVGRECT_HPP

#include "SvgLength.hpp"
#include "SvgShape.hpp"

namespace Elpida
{
	class SvgRect : public SvgShape
	{
	public:
		SvgRect(const XmlElement& element, SvgDocument& document);
	private:
		SvgLength _x;
		SvgLength _y;
		SvgLength _width;
		SvgLength _height;
		SvgLength _rX;
		SvgLength _rY;
	};
} // Elpida

#endif //SVGRECT_HPP
