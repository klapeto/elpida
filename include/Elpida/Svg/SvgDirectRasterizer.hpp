//
// Created by klapeto on 23/6/2024.
//

#ifndef ELPIDA_SVGDIRECTRASTERIZER_HPP
#define ELPIDA_SVGDIRECTRASTERIZER_HPP

#include <cstddef>

namespace Elpida
{

	class SvgCalculatedDocument;
	class SvgBackDrop;

	class SvgDirectRasterizer
	{
	public:
		/**
		 * This is non conforming in any case. Only for testing Memory scaling scenarios.
		 * @param document
		 * @param backDrop
		 * @param subSamples
		 */
		static void Rasterize(const SvgCalculatedDocument& document, SvgBackDrop& backDrop, SvgBackDrop& tempBackDrop,
				std::size_t subSamples = 16);
	};

} // Elpida

#endif //ELPIDA_SVGDIRECTRASTERIZER_HPP
