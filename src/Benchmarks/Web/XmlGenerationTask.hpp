//
// Created by klapeto on 21/5/2024.
//

#ifndef ELPIDA_XMLGENERATIONTASK_HPP
#define ELPIDA_XMLGENERATIONTASK_HPP

#include "Elpida/Core/Task.hpp"

namespace Elpida
{

	class XmlGenerationTask : public Task
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;
		explicit XmlGenerationTask(std::size_t xmlSize);
		~XmlGenerationTask() override = default;
	protected:
		TaskInfo DoGetInfo() const override;
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;
	private:
		std::shared_ptr<AbstractTaskData> _outputData;
		std::size_t _elementCount;
		std::size_t _xmlSize;
	};

} // Elpida

#endif //ELPIDA_XMLGENERATIONTASK_HPP
