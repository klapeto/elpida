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
	class SvgSuperSampler;

	class SvgDirectRasterizer
	{
	public:
		/**
		 * This is non conforming in any case. Only for testing Memory scaling scenarios.
		 * @param document
		 * @param backDrop
		 * @param subSamples
		 */
		static void Rasterize(const SvgCalculatedDocument& document, SvgBackDrop& backDrop,
				std::size_t subSamples = 16);

		static void Rasterize(const SvgCalculatedDocument& document, SvgBackDrop& backDrop,
				const SvgSuperSampler& superSampler);
	};

} // Elpida

#endif //ELPIDA_SVGDIRECTRASTERIZER_HPP
