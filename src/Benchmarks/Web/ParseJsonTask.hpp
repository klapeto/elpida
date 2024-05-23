//
// Created by klapeto on 7/4/2024.
//

#ifndef ELPIDA_PARSEJSONTASK_HPP
#define ELPIDA_PARSEJSONTASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "json.hpp"

namespace Elpida
{
	class ParseJsonTask: public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		ParseJsonTask() = default;
		~ParseJsonTask() override = default;
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
		nlohmann::json _parsedElement;
	};
} // Elpida

#endif //ELPIDA_PARSEJSONTASK_HPP
