//
// Created by klapeto on 31/5/20.
//

#ifndef INCLUDE_BENCHMARKS_IMAGE_IMAGEDATAPROPERTIESTRANSFORMER_HPP
#define INCLUDE_BENCHMARKS_IMAGE_IMAGEDATAPROPERTIESTRANSFORMER_HPP

#include <Elpida/Engine/Data/DataPropertiesTransformer.hpp>

namespace Elpida {
	class ImageDataPropertiesTransformer :public DataPropertiesTransformer
	{
	public:
		[[nodiscard]] std::unordered_map<std::string, double> transform(size_t originalSize,
			const std::unordered_map<std::string, double>& originalProperties,
			size_t targetSize) const override;

		ImageDataPropertiesTransformer() = default;
		~ImageDataPropertiesTransformer() override = default;
	};
}
#endif //INCLUDE_BENCHMARKS_IMAGE_IMAGEDATAPROPERTIESTRANSFORMER_HPP
