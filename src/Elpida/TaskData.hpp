//
// Created by klapeto on 27/2/2023.
//

#ifndef _TASKDATA_HPP_
#define _TASKDATA_HPP_

#include <cstdlib>
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

#include "Elpida/NumaMemory.hpp"
#include "Elpida/Topology/ProcessingUnitNode.hpp"

namespace Elpida
{

	class TaskData final
	{
	 public:
		using Metadata = std::unordered_map<std::string, std::string>;

		Metadata& GetMetadata()
		{
			return _metadata;
		}

		size_t GetSize() const
		{
			return _size;
		}

		char* GetDataRaw() const
		{
			return _data.get();
		}

		const ProcessingUnitNode& GetTargetProcessor() const
		{
			return _targetProcessor;
		}

		bool IsAllocated() const
		{
			return _data.get() != nullptr;
		}

		void Allocate(std::size_t size);
		void Deallocate();
		void Migrate(const ProcessingUnitNode& targetProcessor);
		std::vector<TaskData>
		Split(const std::vector<std::reference_wrapper<const ProcessingUnitNode>>& targetProcessors, std::size_t chunkDivisibleBy = 1);
		void Merge(const std::vector<TaskData>& chunks);

		explicit TaskData(const ProcessingUnitNode& targetProcessor);
		TaskData(const TaskData&) = delete;
		TaskData(TaskData&& other) noexcept;
		TaskData& operator=(const TaskData&) = delete;
		TaskData& operator=(TaskData&& other) noexcept;
		~TaskData() = default;
	 private:
		Metadata _metadata;
		NumaUniquePtr _data;
		std::reference_wrapper<const ProcessingUnitNode> _targetProcessor;
		std::size_t _size;

		int GetNumaNodeId() const;

		static std::vector<TaskData>
		SplitChunksToChunks(const std::vector<const TaskData*>& input, const std::vector<std::reference_wrapper<const ProcessingUnitNode>>& targetProcessors, std::size_t chunkDivisibleBy);
	};

} // Elpida

#endif //_TASKDATA_HPP_
