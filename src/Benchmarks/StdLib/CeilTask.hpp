#ifndef ELPIDA_CEILTASK_HPP
#define ELPIDA_CEILTASK_HPP

#include "Elpida/Core/MicroTask.hpp"

namespace Elpida
{

	class CeilTask : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		CeilTask() = default;
		~CeilTask() override = default;
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
	};

}

#endif //ELPIDA_CEILTASK_HPP
