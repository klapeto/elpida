#[[#ifndef]]# ${INCLUDE_GUARD}
#[[#define]]# ${INCLUDE_GUARD}

#[[#include]]# "Elpida/Core/MicroTask.hpp"

namespace Elpida {

class ${NAME}Task : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		${NAME}Task() = default;
		~${NAME}Task() override = default;
	protected:
		void DoRunImpl() override;

		[[nodiscard]]
		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		SharedPtr<AbstractTaskData> _inputData;
};

}

#[[#endif]]# //${INCLUDE_GUARD}
