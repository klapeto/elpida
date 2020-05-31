//
// Created by klapeto on 30/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_DATA_DATAPROPERTIESTRANSFORMER_HPP
#define INCLUDE_ELPIDA_ENGINE_DATA_DATAPROPERTIESTRANSFORMER_HPP

#include <unordered_map>

namespace Elpida
{
	class DataPropertiesTransformer
	{
	public:
		[[nodiscard]] virtual std::unordered_map<std::string, double> transform(size_t originalSize,
			const std::unordered_map<std::string, double>& originalProperties,
			size_t targetSize) const = 0;

		DataPropertiesTransformer() = default;
		virtual ~DataPropertiesTransformer() = default;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_DATA_DATAPROPERTIESTRANSFORMER_HPP
