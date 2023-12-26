//
// Created by klapeto on 23/12/2023.
//

#ifndef ELPIDA_SVG_SVGRECT_HPP
#define ELPIDA_SVG_SVGRECT_HPP

#include "SvgLength.hpp"
#include "SvgPath.hpp"
#include "SvgShape.hpp"

namespace Elpida
{
	class SvgRect : public SvgPath
	{
	public:
		SvgRect(const XmlElement& element, SvgDocument& document);
	private:
		double _x;
		double _y;
		double _width;
		double _height;
		double _rX;
		double _rY;
	};
} // Elpida

#endif //SVGRECT_HPP
