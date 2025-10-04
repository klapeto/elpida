/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
