#ifndef ELPIDA_FLOORTASK_HPP
#define ELPIDA_FLOORTASK_HPP

#include "Elpida/Core/MicroTask.hpp"

namespace Elpida
{

	class FloorTask : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		FloorTask() = default;
		~FloorTask() override = default;
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

#endif //ELPIDA_FLOORTASK_HPP
