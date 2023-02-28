//
// Created by klapeto on 28/2/2023.
//

#ifndef _MEMORYREADBANDWIDTHTASK_HPP_
#define _MEMORYREADBANDWIDTHTASK_HPP_

#include "Elpida/MicroTask.hpp"
#include <cstdlib>

namespace Elpida
{
	class MemoryReadBandwidthTask: public MicroTask
	{
	 public:
		void Prepare(TaskData&& inputData) override;
		TaskData Finalize() override;
		bool CanBeMultiThreaded() const override;
		TaskInfo GetInfo() const override;
		explicit MemoryReadBandwidthTask(std::size_t size);
		~MemoryReadBandwidthTask() override = default;
	 protected:
		void DoRun(std::size_t iterations) override;
		size_t GetOperationsPerformedPerRun() override;
		Duration GetExecutionMinimumDuration() override;
		std::unique_ptr<Task> DoDuplicate() const override;
	 private:
		TaskData _data;
		char* _ptr;
		std::size_t _size;
	};

} // Elpida

#endif //_MEMORYREADBANDWIDTHTASK_HPP_
