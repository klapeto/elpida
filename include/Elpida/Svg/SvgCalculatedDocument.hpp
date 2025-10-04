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
// Created by klapeto on 1/4/2024.
//

#ifndef ELPIDA_SVGCALCULATEDDOCUMENT_HPP
#define ELPIDA_SVGCALCULATEDDOCUMENT_HPP

#include "Elpida/Svg/SvgCalculatedShape.hpp"
#include "Elpida/Svg/SvgViewBox.hpp"
#include "Elpida/Svg/SvgCalculatedViewPort.hpp"
#include "Elpida/Svg/SvgConfig.hpp"

namespace Elpida
{
	class SvgDocument;

	class SvgCalculatedDocument
	{
	public:
		[[nodiscard]]
		const SvgCalculatedShape& GetRootShape() const
		{
			return _rootShape;
		}


		[[nodiscard]]
		SvgCalculatedShape& GetRootShape()
		{
			return _rootShape;
		}

		[[nodiscard]]
		const SvgCalculatedViewPort& GetViewPort() const
		{
			return _viewPort;
		}

		SvgCalculatedDocument() = default;
		explicit SvgCalculatedDocument(const SvgDocument& document, SvgFloat scale = 1.0);
	private:
		SvgCalculatedShape _rootShape;
		SvgCalculatedViewPort _viewPort;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDDOCUMENT_HPP
