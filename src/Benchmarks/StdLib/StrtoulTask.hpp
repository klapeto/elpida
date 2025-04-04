#ifndef ELPIDA_STRTOULTASK_HPP
#define ELPIDA_STRTOULTASK_HPP

#include "Elpida/Core/MicroTask.hpp"

namespace Elpida
{

	class StrtoulTask : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		StrtoulTask() = default;
		~StrtoulTask() override = default;
	protected:
		void DoRunImpl() override;

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

#endif //ELPIDA_STRTOULTASK_HPP
