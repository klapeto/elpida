//
// Created by klapeto on 28/2/2023.
//

#ifndef _MEMORYREADBANDWIDTHTASK_HPP_
#define _MEMORYREADBANDWIDTHTASK_HPP_

#include <cstdlib>
#include <optional>

#include "Elpida/MicroTask.hpp"

namespace Elpida
{
	class MemoryReadBandwidthTask : public MicroTask
	{
	 public:
		void Prepare(RawTaskData&& inputData) override;
		RawTaskData Finalize() override;
		bool CanBeMultiThreaded() const override;
		TaskInfo GetInfo() const override;
		explicit MemoryReadBandwidthTask(std::size_t size);
		~MemoryReadBandwidthTask() override = default;
	 protected:
		void DoRun(std::size_t iterations) override;
		std::size_t GetOperationsPerformedPerRun() override;
		Duration GetExecutionMinimumDuration() override;
		std::unique_ptr<Task> DoDuplicate() const override;
	 private:
		std::optional<RawTaskData> _data;
		char* _ptr;
		std::size_t _size;
	};

} // Elpida

#endif //_MEMORYREADBANDWIDTHTASK_HPP_
