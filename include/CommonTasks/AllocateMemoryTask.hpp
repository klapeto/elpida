//
// Created by klapeto on 2/6/2024.
//

#ifndef ELPIDA_ALLOCATEMEMORYTASK_HPP
#define ELPIDA_ALLOCATEMEMORYTASK_HPP

#include "Elpida/Core/Task.hpp"

namespace Elpida
{

	class AllocateMemoryTask: public Task
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		ConcurrencyMode GetAllowedConcurrency() const override;
		Size GetProcessedDataSize() const override;

		explicit AllocateMemoryTask(std::size_t size);
		~AllocateMemoryTask() override = default;
	protected:
		void DoRun() override;
		std::unique_ptr<Task> DoDuplicate() const override;
		TaskInfo DoGetInfo() const override;
	private:
		SharedPtr<AbstractTaskData> _outputData;
		std::size_t _size;
	};

} // Elpida

#endif //ELPIDA_ALLOCATEMEMORYTASK_HPP
