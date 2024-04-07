//
// Created by klapeto on 7/4/2024.
//

#ifndef ELPIDA_PARSEXMLTASK_HPP
#define ELPIDA_PARSEXMLTASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Xml/XmlElement.hpp"

namespace Elpida
{
	class ParseXmlTask : public MicroTask
	{
	public:

		void Prepare(UniquePtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		bool CanBeMultiThreaded() const override;

		[[nodiscard]]
		UniquePtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		TaskInfo GetInfo() const override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		ParseXmlTask() = default;
		~ParseXmlTask() override = default;
	protected:
		void DoRun(Iterations iterations) override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		Duration GetExecutionMinimumDuration() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		UniquePtr<AbstractTaskData> _inputData;
		XmlElement _parsedElement;
	};

} // Elpida

#endif //ELPIDA_PARSEXMLTASK_HPP
