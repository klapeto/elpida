//
// Created by klapeto on 24/12/2023.
//

#ifndef ELPIDA_SVG_SVGRASTERIZER_HPP
#define ELPIDA_SVG_SVGRASTERIZER_HPP

#include <cstddef>

#include "Elpida/Svg/SvgBackDrop.hpp"
#include "Elpida/Svg/SvgTransform.hpp"

namespace Elpida
{
	class SvgDocument;
	class SvgViewBox;
	class SvgCalculatedShape;
	class SvgTransform;

	class SvgRasterizer
	{
	public:
		SvgBackDrop Rasterize(const SvgDocument& document, double scale = 1.0, std::size_t subSamples = 16);
	private:
		void RasterizeShape(SvgBackDrop& backDrop, const SvgCalculatedShape& shape, const SvgTransform& transform);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZER_HPP
