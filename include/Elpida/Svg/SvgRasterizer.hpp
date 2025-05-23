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

	class SvgRasterizer
	{
	public:
		static SvgBackDrop Rasterize(const SvgCalculatedDocument& document, const SvgSuperSampler& superSampler, bool multiThread);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZER_HPP
