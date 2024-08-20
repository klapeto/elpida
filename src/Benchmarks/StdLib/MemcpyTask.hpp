#ifndef ELPIDA_MEMCPYTASK_HPP
#define ELPIDA_MEMCPYTASK_HPP

#include "Elpida/Core/MicroTask.hpp"

namespace Elpida
{

	class MemcpyTask : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		MemcpyTask() = default;
		~MemcpyTask() override = default;
	protected:
		void DoRun(Iterations iterations) override;

		[[nodiscard]]
		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		Duration GetExecutionMinimumDuration() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		SharedPtr<AbstractTaskData> _inputData;
		SharedPtr<AbstractTaskData> _outputData;
	};

}

#endif //ELPIDA_MEMCPYTASK_HPP
