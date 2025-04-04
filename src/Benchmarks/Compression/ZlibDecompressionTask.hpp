//
// Created by klapeto on 31/7/2024.
//

#ifndef ELPIDA_ZLIBDECOMPRESSIONTASK_HPP
#define ELPIDA_ZLIBDECOMPRESSIONTASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Core/MemoryStream.hpp"

#include "zlib.h"

#include <optional>

namespace Elpida
{

	class ZlibDecompressionTask: public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		ZlibDecompressionTask();
		~ZlibDecompressionTask() override ;
	protected:
		void DoRunImpl() override;

		[[nodiscard]]
		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		z_stream _zStream;
		SharedPtr<AbstractTaskData> _input;
		std::optional<MemoryStream> _output;
	};

} // Elpida

#endif //ELPIDA_ZLIBDECOMPRESSIONTASK_HPP
