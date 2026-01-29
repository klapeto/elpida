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
// Created by klapeto on 7/9/2024.
//

#ifndef ELPIDA_UTILITIES_HPP
#define ELPIDA_UTILITIES_HPP

#include "Elpida/Svg/SvgCalculatedDocument.hpp"

namespace Elpida
{
	class Utilities
	{
	public:
		static std::size_t CalculateTotalPixelsRendered(const SvgCalculatedShape& shape)
		{
			std::size_t size = 0;
			if (!shape.GetPaths().empty() && shape.GetOpacity() > 0.0)
			{
				std::size_t shapeSize = std::ceil(shape.GetBounds().GetHeight()) * std::ceil((shape.GetBounds().GetWidth()));
				if (shape.GetFill().has_value())
				{
					size += shapeSize;
				}

				if (shape.GetStroke().has_value())
				{
					size += shapeSize;
				}
			}

			for (auto& child : shape.GetChildren())
			{
				size += CalculateTotalPixelsRendered(child);
			}

			return size;
		}
	};
}

#endif //ELPIDA_UTILITIES_HPP
