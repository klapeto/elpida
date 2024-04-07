//
// Created by klapeto on 28/2/2023.
//

#ifndef _MEMORYREADBANDWIDTHTASK_HPP_
#define _MEMORYREADBANDWIDTHTASK_HPP_

#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/MicroTask.hpp"

namespace Elpida
{
	class MemoryReadBandwidthTask : public MicroTask
	{
	 public:
		void Prepare(UniquePtr<AbstractTaskData> inputData) override;
		UniquePtr<AbstractTaskData> Finalize() override;
		bool CanBeMultiThreaded() const override;
		Size GetProcessedDataSize() const override;
		explicit MemoryReadBandwidthTask(Size size);
		~MemoryReadBandwidthTask() override = default;
	 protected:
		void DoRun(Iterations iterations) override;
		Size GetOperationsPerformedPerRun() override;
		Duration GetExecutionMinimumDuration() override;
		UniquePtr<Task> DoDuplicate() const override;
		TaskInfo DoGetInfo() const override;
	 private:
		UniquePtr<AbstractTaskData> _data;
		unsigned char* _ptr;
		Size _size;
	};

} // Elpida

#endif //_MEMORYREADBANDWIDTHTASK_HPP_
