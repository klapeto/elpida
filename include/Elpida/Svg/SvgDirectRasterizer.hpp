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
