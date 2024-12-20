//
// Created by klapeto on 20/12/2024.
//

#ifndef ELPIDA_GENERATECPPCODETASK_HPP
#define ELPIDA_GENERATECPPCODETASK_HPP

#include "Elpida/Core/Task.hpp"

namespace Elpida
{

	class GenerateCppCodeTask: public Task
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;
		explicit GenerateCppCodeTask() = default;
		~GenerateCppCodeTask() override = default;
	protected:
		TaskInfo DoGetInfo() const override;
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;
	private:
		std::shared_ptr<AbstractTaskData> _outputData;
	};
} // Elpida

#endif //ELPIDA_GENERATECPPCODETASK_HPP
