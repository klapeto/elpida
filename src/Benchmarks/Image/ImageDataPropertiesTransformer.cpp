//
// Created by klapeto on 31/5/20.
//

#include "Benchmarks/Image/ImageDataPropertiesTransformer.hpp"

namespace Elpida
{

	std::unordered_map<std::string, double> ImageDataPropertiesTransformer::transform(size_t originalSize,
		const std::unordered_map<std::string, double>& originalProperties,
		size_t targetSize) const
	{
		auto width = originalProperties.at("width");
		auto height = originalProperties.at("height");

		auto ratio = originalSize / targetSize;

		auto targetWidth = width;	// This probably won't work for all cases
		auto targetHeight = height / ratio;

		return {
			{ "width", targetWidth },
			{ "height", targetHeight }
		};
	}
}