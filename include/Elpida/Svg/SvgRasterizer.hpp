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
			std::size_t width,
			std::size_t height,
			std::size_t stride,
			double tx = 0.0,
			double ty = 0.0,
			double scale = 1.0,
			std::size_t subSamples = 16);
	private:
		static constexpr std::size_t SubSamples = 5;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZER_HPP
