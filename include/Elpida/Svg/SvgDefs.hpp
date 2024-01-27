//
// Created by klapeto on 22/12/2023.
//

#ifndef ELPIDA_SVG_SVGDEFS_HPP
#define ELPIDA_SVG_SVGDEFS_HPP

#include <unordered_map>

#include "SvgGradient.hpp"

namespace Elpida
{
	class SvgDefs : public std::unordered_map<std::string, SvgGradient>
	{
	};
} // Elpida

#endif //ELPIDA_SVG_SVGDEFS_HPP
