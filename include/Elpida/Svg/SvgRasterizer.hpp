//
// Created by klapeto on 24/12/2023.
//

#ifndef ELPIDA_SVG_SVGRASTERIZER_HPP
#define ELPIDA_SVG_SVGRASTERIZER_HPP

#include <cstddef>

#include "Elpida/Svg/SvgBackDrop.hpp"

namespace Elpida
{
	class SvgCalculatedDocument;
	class ThreadPool;

	class SvgRasterizer
	{
	public:
		static SvgBackDrop Rasterize(const SvgCalculatedDocument& document, std::size_t subSamples = 16);
		static SvgBackDrop RasterizeCustom(const SvgCalculatedDocument& document, std::size_t subSamples, bool multiThreadedShapes, bool multiThreadedSuperSampling, bool multiThreadedBitBlit, ThreadPool& threadPool);
		static SvgBackDrop RasterizeMultiThreaded(const SvgCalculatedDocument& document, ThreadPool& threadPool, std::size_t subSamples = 16);

	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZER_HPP
