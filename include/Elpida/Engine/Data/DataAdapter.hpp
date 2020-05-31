//
// Created by klapeto on 10/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATAADAPTER_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATAADAPTER_HPP

#include <vector>

namespace Elpida
{
	class Task;
	class TaskAffinity;
	class DataSpecification;
	class RawData;

	class DataAdapter
	{
	public:
		static void adaptAndForwardTaskData(Task* previous, Task* next);

		[[nodiscard]] static std::vector<RawData*> breakIntoChunks(const RawData& input,
			const TaskAffinity& affinity,
			const DataSpecification& targetDataSpecification);

		[[nodiscard]] static RawData* mergeIntoSingleChunk(const std::vector<const RawData*>& inputData);

		virtual ~DataAdapter() = default;
	protected:
		static size_t getAccumulatedSizeOfChunks(const std::vector<const RawData*>& outputChunks);
		static std::vector<RawData*> breakIntoChunksImpl(const std::vector<const RawData*>& input,
			const std::vector<int>& processorsOsIndices,
			size_t chunksDivisibleBy);
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATAADAPTER_HPP
