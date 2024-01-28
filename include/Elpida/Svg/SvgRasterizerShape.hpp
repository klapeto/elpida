//
// Created by klapeto on 22/1/2024.
//

#ifndef ELPIDA_SVG_SVGRASTERIZERSHAPE_HPP
#define ELPIDA_SVG_SVGRASTERIZERSHAPE_HPP

#include <vector>

#include "SvgRasterizerEdge.hpp"

namespace Elpida
{
	class SvgRasterizerShape
	{
	public:

	private:
		std::vector<SvgRasterizerEdge> _fillEdges;
		std::vector<SvgRasterizerEdge> _strokeEdges;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZERSHAPE_HPP
