//
// Created by klapeto on 3/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGPATH_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGPATH_HPP

#include <vector>
namespace Elpida
{

	class SvgPath
	{

	private:
		std::vector<float> _points;
		float _bounds[4];
		bool _closed;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGPATH_HPP
