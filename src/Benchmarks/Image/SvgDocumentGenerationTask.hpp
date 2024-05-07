//
// Created by klapeto on 7/5/2024.
//

#ifndef ELPIDA_SVGDOCUMENTGENERATIONTASK_HPP
#define ELPIDA_SVGDOCUMENTGENERATIONTASK_HPP

#include "Elpida/Core/Task.hpp"

namespace Elpida
{
	class SvgDocumentGenerationTask: public Task
	{
	public:
		void Prepare(UniquePtr<AbstractTaskData> inputData) override;
		UniquePtr<AbstractTaskData> Finalize() override;
		bool CanBeMultiThreaded() const override;
		Size GetProcessedDataSize() const override;

		explicit SvgDocumentGenerationTask(std::size_t elementCount);
		~SvgDocumentGenerationTask() override = default;
	protected:
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;
		TaskInfo DoGetInfo() const override;
	private:
		std::unique_ptr<AbstractTaskData> _outputData;
		std::size_t _elementCount;
	};

} // Elpida

#endif //ELPIDA_SVGDOCUMENTGENERATIONTASK_HPP
