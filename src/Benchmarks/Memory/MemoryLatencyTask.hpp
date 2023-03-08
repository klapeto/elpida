//
// Created by klapeto on 27/2/2023.
//

#ifndef _MEMORYLATENCYTASK_HPP_
#define _MEMORYLATENCYTASK_HPP_

#include "Elpida/UniquePtr.hpp"
#include "Elpida/Size.hpp"
#include "Elpida/MicroTask.hpp"

namespace Elpida
{

	class MemoryLatencyTask : public MicroTask
	{
	 public:

		void Prepare(UniquePtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		bool CanBeMultiThreaded() const override;

		[[nodiscard]]
		UniquePtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		TaskInfo GetInfo() const override;

		MemoryLatencyTask(Size size, Size cacheLineSize, Size pageSize);
		~MemoryLatencyTask() override = default;
	 protected:
		void DoRun(Size iterations) override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		Duration GetExecutionMinimumDuration() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	 private:
		UniquePtr<AbstractTaskData> _data;
		char* _ptr;
		Size _size;
		Size _cacheLineSize;
		Size _pageSize;
	};

} // Elpida

#endif //_MEMORYLATENCYTASK_HPP_
