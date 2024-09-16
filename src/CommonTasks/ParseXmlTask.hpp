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

		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		ParseXmlTask() = default;
		~ParseXmlTask() override = default;
	protected:
		void DoRun(Iterations iterations) override;

		[[nodiscard]]
		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		SharedPtr<AbstractTaskData> _inputData;
		XmlElement _parsedElement;
	};

} // Elpida

#endif //ELPIDA_PARSEXMLTASK_HPP
