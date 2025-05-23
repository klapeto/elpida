//
// Created by klapeto on 28/2/2023.
//

#ifndef _MEMORYREADBANDWIDTHTASK_HPP_
#define _MEMORYREADBANDWIDTHTASK_HPP_

#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/IterationMicroTask.hpp"

namespace Elpida
{
	class MemoryReadBandwidthTask : public IterationMicroTask
	{
	 public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;
		explicit MemoryReadBandwidthTask();
		~MemoryReadBandwidthTask() override = default;
	 protected:
		void DoRun(Iterations iterations) override;
		Size GetOperationsPerformedPerRun() override;
		Duration GetExecutionMinimumDuration() override;
		UniquePtr<Task> DoDuplicate() const override;
		TaskInfo DoGetInfo() const override;
	 private:
		SharedPtr<AbstractTaskData> _data;
		unsigned char* _ptr;
	};

} // Elpida

#endif //_MEMORYREADBANDWIDTHTASK_HPP_
