//
// Created by klapeto on 3/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGDOCUMENT_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGDOCUMENT_HPP

#include "SvgPaintType.hpp"
#include "SvgShape.hpp"
#include "SvgUnits.hpp"
#include <vector>
namespace Elpida
{

	class SvgDocument
	{
	public:
		void Load(const char* data, std::size_t size, SvgUnits units, float dpi);
	private:
		std::vector<SvgShape> _shapes;
		float _width;
		float _height;
		void ParseElement(const char* data, std::size_t& i, std::size_t size);
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGDOCUMENT_HPP
