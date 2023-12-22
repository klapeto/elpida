//
// Created by klapeto on 22/12/2023.
//

#ifndef ELPIDA_SVG_SVGDEFS_HPP
#define ELPIDA_SVG_SVGDEFS_HPP

#include <memory>
#include <unordered_map>

#include "SvgElement.hpp"

namespace Elpida
{
	class SvgDefs : public std::unordered_map<std::string, std::unique_ptr<SvgElement>>
	{
	public:
		SvgDefs() = default;
		~SvgDefs() = default;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGDEFS_HPP
