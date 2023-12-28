//
// Created by klapeto on 24/12/2023.
//

#ifndef ELPIDA_SVG_SVGRASTERIZER_HPP
#define ELPIDA_SVG_SVGRASTERIZER_HPP
#include <cstddef>


namespace Elpida
{
	class SvgDocument;

	class SvgRasterizer
	{
	public:
		void Rasterize(const SvgDocument& document,
			unsigned char* outputBuffer,
			std::size_t bufferSize,
			std::size_t width,
			std::size_t height,
			std::size_t stride,
			double scale = 1.0);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZER_HPP