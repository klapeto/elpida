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
// Created by klapeto on 22/12/2023.
//

#ifndef ELPIDA_SVG_SVGDEFS_HPP
#define ELPIDA_SVG_SVGDEFS_HPP

#include <unordered_map>
#include <string>

#include "SvgGradient.hpp"

namespace Elpida
{
	class SvgDefs : public std::unordered_map<std::string, SvgGradient>
	{
	};
} // Elpida

#endif //ELPIDA_SVG_SVGDEFS_HPP
