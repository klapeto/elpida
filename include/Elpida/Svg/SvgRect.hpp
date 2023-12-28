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
	};
} // Elpida

#endif //SVGRECT_HPP
