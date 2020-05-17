//
// Created by klapeto on 10/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_DATA_ADAPTERS_COPYINGCHUNKNORMALIZERADAPTER_HPP
#define INCLUDE_ELPIDA_ENGINE_DATA_ADAPTERS_COPYINGCHUNKNORMALIZERADAPTER_HPP

#include "Elpida/Engine/Data/DataAdapter.hpp"

namespace Elpida
{
	/**
	 * This adapter copies the output data to new memory regions for each of
	 * the processors defined by the TaskAffinity. The outcome TaskInput will
	 * have total size of at least the TaskOutput total size and may be some
	 * additional padding zeroed bytes on the last chunk if the TaskOutput total
	 * size is not divisible by the TaskAffinity processor count
	 *
	 * @note TaskData in the TaskInput produced is ActiveTaskData
	 * @attention New memory regions are allocated for each chunk created. Keep this in
	 * mind if you are dealing with large amounts of memory
	 */
	class CopyingChunkNormalizerAdapter : public DataAdapter
	{
	public:
		[[nodiscard]] TaskInput transformOutputToInput(const TaskOutput& output,
			const TaskAffinity& affinity,
			const DataSpecification& inputDataSpecification) const override;

		CopyingChunkNormalizerAdapter() = default;
		~CopyingChunkNormalizerAdapter() override = default;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_DATA_ADAPTERS_COPYINGCHUNKNORMALIZERADAPTER_HPP
