#ifndef ELPIDA_MEMSETTASK_HPP
#define ELPIDA_MEMSETTASK_HPP

#include "Elpida/Core/MicroTask.hpp"

namespace Elpida
{

	class MemsetTask : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		MemsetTask() = default;
		~MemsetTask() override = default;
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

#endif //ELPIDA_MEMSETTASK_HPP
