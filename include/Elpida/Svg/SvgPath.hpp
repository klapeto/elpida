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

#ifndef ELPIDA_SVGPATH_HPP
#define ELPIDA_SVGPATH_HPP

#include "SvgPathInstance.hpp"

#include <string>

namespace Elpida
{
	class SvgCalculationContext;
	class XmlMap;

	class SvgPath
	{
	public:
		[[nodiscard]]
		std::vector<SvgPathInstance> CalculatePaths() const;

		explicit SvgPath(const XmlMap& properties);
	private:
		std::string _pathData;
	};

} // Elpida

#endif //ELPIDA_SVGPATH_HPP
