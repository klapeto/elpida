//
// Created by klapeto on 30/7/2024.
//

#ifndef ELPIDA_ZLIBCOMPRESSIONTASK_HPP
#define ELPIDA_ZLIBCOMPRESSIONTASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Core/MemoryStream.hpp"

#include <optional>

namespace Elpida
{

	class ZlibCompressionTask : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		ZlibCompressionTask() = default;
		~ZlibCompressionTask() override = default;
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
		SharedPtr<AbstractTaskData> _input;
		std::optional<MemoryStream> _output;
	};

} // Elpida

#endif //ELPIDA_ZLIBCOMPRESSIONTASK_HPP
