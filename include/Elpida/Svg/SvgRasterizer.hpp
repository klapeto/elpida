//
// Created by klapeto on 24/12/2023.
//

#ifndef ELPIDA_SVG_SVGRASTERIZER_HPP
#define ELPIDA_SVG_SVGRASTERIZER_HPP

#include <cstddef>
#include <future>

#include "Elpida/Svg/SvgBackDrop.hpp"
#include "Elpida/Svg/SvgTransform.hpp"
#include "Elpida/Svg/SvgBounds.hpp"

namespace Elpida
{
	class SvgCalculatedDocument;
	class SvgViewBox;
	class SvgCalculatedShape;
	class SvgTransform;
	class ThreadPool;

	class SvgRasterizer
	{
	public:
		SvgBackDrop Rasterize(SvgCalculatedDocument& document, std::size_t subSamples = 16);
		SvgBackDrop RasterizeMultiThreaded(SvgCalculatedDocument& document, ThreadPool& threadPool, std::size_t subSamples = 16);

	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZER_HPP
