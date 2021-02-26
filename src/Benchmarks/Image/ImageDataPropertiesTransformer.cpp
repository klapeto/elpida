/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 31/5/20.
//

#include <cmath>
#include "Benchmarks/Image/ImageDataPropertiesTransformer.hpp"

namespace Elpida
{

	std::unordered_map<std::string, double> ImageDataPropertiesTransformer::transformDataProperties(size_t originalSize,
		const std::unordered_map<std::string, double>& originalProperties,
		size_t targetSize, float targetRatioToOriginal) const
	{
		auto width = originalProperties.at("width");
		auto stride = originalProperties.at("stride");
		auto fullSize = targetSize / targetRatioToOriginal;

		auto targetWidth = width;
		auto ratio = originalSize / (float)fullSize;
		auto targetStride = std::round(stride / ratio);
		auto targetHeight = std::round(targetSize / targetStride);

		return {
			{ "width", targetWidth },
			{ "height", targetHeight },
			{ "stride", targetStride }
		};
	}
}