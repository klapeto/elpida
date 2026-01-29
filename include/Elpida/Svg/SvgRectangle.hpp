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
// Created by klapeto on 8/3/2024.
//

#ifndef ELPIDA_SVGRECTANGLE_HPP
#define ELPIDA_SVGRECTANGLE_HPP

#include "SvgLength.hpp"
#include "SvgPathInstance.hpp"

#include <optional>

namespace Elpida
{
	class XmlMap;
	class SvgCalculationContext;

	class SvgRectangle
	{
	public:
		[[nodiscard]]
		std::vector<SvgPathInstance> CalculatePaths(const SvgCalculationContext& calculationContext) const;

		explicit SvgRectangle(const XmlMap& properties);
	private:
		SvgLength _x;
		SvgLength _y;
		SvgLength _width;
		SvgLength _height;
		std::optional<SvgLength> _rX;
		std::optional<SvgLength> _rY;
	};

} // Elpida

#endif //ELPIDA_SVGRECTANGLE_HPP
