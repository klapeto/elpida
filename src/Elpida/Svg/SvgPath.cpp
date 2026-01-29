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

#include "Elpida/Svg/SvgPath.hpp"
#include "Elpida/Svg/SvgPathGenerator.hpp"
#include "Elpida/Xml/XmlMap.hpp"

namespace Elpida
{
	SvgPath::SvgPath(const XmlMap& properties)
	{
		_pathData = properties.GetValue("d");
	}

	std::vector<SvgPathInstance> SvgPath::CalculatePaths() const
	{
		SvgPathGenerator generator;
		generator.ParsePathData(_pathData);
		return std::move(generator.GetPaths());
	}
} // Elpida