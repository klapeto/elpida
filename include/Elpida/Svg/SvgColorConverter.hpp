//
// Created by klapeto on 7/3/2024.
//

#ifndef ELPIDA_SVGCOLORCONVERTER_HPP
#define ELPIDA_SVGCOLORCONVERTER_HPP

#include <vector>
#include <memory>
#include <limits>
#include <cstring>
#include "SvgColor.hpp"

namespace Elpida
{

	class SvgColorConverter
	{
	public:
		template<typename T>
		static std::unique_ptr<T[]> Convert(const std::vector<SvgColor>& colorData)
		{
			std::unique_ptr<T[]> data(new T[colorData.size() * 4]);

			std::memset(data.get(), 0, colorData.size() * 4);

			for (std::size_t i = 0; i < colorData.size(); ++i)
			{
				auto colorIndex = i * 4;
				auto& color = colorData[i];
				data[colorIndex] = color.R() * std::numeric_limits<T>::max();
				data[colorIndex + 1] = color.G() * std::numeric_limits<T>::max();
				data[colorIndex + 2] = color.B() * std::numeric_limits<T>::max();
				data[colorIndex + 3] = color.A() * std::numeric_limits<T>::max();
			}

			return data;
		}
	};

} // Elpida

#endif //ELPIDA_SVGCOLORCONVERTER_HPP
