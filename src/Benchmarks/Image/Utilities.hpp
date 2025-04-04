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
