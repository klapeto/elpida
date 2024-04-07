//
// Created by klapeto on 7/4/2024.
//

#ifndef ELPIDA_SVGPARSETASK_HPP
#define ELPIDA_SVGPARSETASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Svg/SvgDocument.hpp"
#include "Elpida/Xml/XmlElement.hpp"

namespace Elpida
{

	class SvgParseTask : public MicroTask
	{
	public:

		void Prepare(UniquePtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		bool CanBeMultiThreaded() const override;

		[[nodiscard]]
		UniquePtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		SvgParseTask() = default;
		~SvgParseTask() override = default;
	protected:
		void DoRun(Iterations iterations) override;

		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		Duration GetExecutionMinimumDuration() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		UniquePtr<AbstractTaskData> _inputData;
		XmlElement _inputXmlElement;
		SvgDocument _parsedElement;
	};

} // Elpida

#endif //ELPIDA_SVGPARSETASK_HPP
