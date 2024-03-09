//
// Created by klapeto on 24/12/2023.
//

#ifndef ELPIDA_SVG_SVGRASTERIZER_HPP
#define ELPIDA_SVG_SVGRASTERIZER_HPP

#include <cstddef>
#include "SvgBackDrop.hpp"

namespace Elpida
{
	class SvgDocument;

	class SvgRasterizer
	{
	public:
		SvgBackDrop Rasterize(const SvgDocument& document, double scale = 1.0, std::size_t subSamples = 16);
	private:
		static constexpr std::size_t SubSamples = 5;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZER_HPP
