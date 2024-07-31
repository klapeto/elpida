//
// Created by klapeto on 21/5/2024.
//

#ifndef ELPIDA_JSONGENERATIONTASK_HPP
#define ELPIDA_JSONGENERATIONTASK_HPP

#include "Elpida/Core/Task.hpp"

namespace Elpida
{

	class JsonGenerationTask : public Task
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;
		explicit JsonGenerationTask(std::size_t jsonSize);
		~JsonGenerationTask() override = default;
	protected:
		TaskInfo DoGetInfo() const override;
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;
	private:
		std::shared_ptr<AbstractTaskData> _outputData;
		std::size_t _elementCount;
		std::size_t _jsonSize;
	};

} // Elpida

#endif //ELPIDA_JSONGENERATIONTASK_HPP
