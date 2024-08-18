#ifndef ELPIDA_MALLOCTASK_HPP
#define ELPIDA_MALLOCTASK_HPP

#include "Elpida/Core/MicroTask.hpp"

#include <vector>

namespace Elpida
{

	class MallocTask : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		MallocTask();
		~MallocTask() override = default;
	protected:
		void DoRun(Iterations iterations) override;

		void OnBeforeRun(Iterations iterations) override;

		[[nodiscard]]
		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		Duration GetExecutionMinimumDuration() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		std::vector<void*> _pointers;
		std::size_t _totalSize;
		std::vector<std::size_t> _sizes;
		SharedPtr<AbstractTaskData> _inputData;
	};

}

#endif //ELPIDA_MALLOCTASK_HPP
