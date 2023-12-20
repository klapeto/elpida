//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGPATH_HPP
#define ELPIDA_SVG_SVGPATH_HPP

#include "SvgBounds.hpp"
#include <vector>
namespace Elpida
{

	class SvgPath
	{

	private:
		std::vector<float> _points;
		SvgBounds _bounds;
		bool _closed;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGPATH_HPP
