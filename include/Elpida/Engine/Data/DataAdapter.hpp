//
// Created by klapeto on 10/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATAADAPTER_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATAADAPTER_HPP

#include "Elpida/Engine/Task/TaskInput.hpp"

namespace Elpida
{
	class TaskOutput;
	class TaskAffinity;
	class DataSpecification;

	class DataAdapter
	{
	public:

		[[nodiscard]] virtual std::vector<RawData*> breakIntoChunks(const RawData& input,
			const TaskAffinity& affinity,
			const DataSpecification& targetDataSpecification) const = 0;

		[[nodiscard]] virtual RawData* mergeIntoSingleChunk(const std::vector<const RawData*>& inputData) const = 0;

		virtual ~DataAdapter() = default;
	protected:
		static size_t getAccumulatedSizeOfChunks(const std::vector<const RawData*>& outputChunks);
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATAADAPTER_HPP
