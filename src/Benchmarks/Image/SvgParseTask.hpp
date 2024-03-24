//
// Created by klapeto on 31/10/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVGPARSETASK_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVGPARSETASK_HPP

#include "Elpida/Core/Allocator.hpp"
#include "Elpida/Core/MicroTask.hpp"

#include "Elpida/Core/SharedPtr.hpp"
#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"
#include "Elpida/Xml/XmlElement.hpp"
#include "Elpida/Xml/XmlParser.hpp"

namespace Elpida
{

	class SvgParseTask final: public MicroTask
	{
	public:
		void Prepare(UniquePtr<AbstractTaskData> inputData) override;
		UniquePtr<AbstractTaskData> Finalize() override;
		TaskInfo GetInfo() const override;
		bool CanBeMultiThreaded() const override;
		Size GetProcessedDataSize() const override;
		SvgParseTask() = default;
		~SvgParseTask() override = default;
	protected:
		void DoRun(Iterations iterations) override;
		UniquePtr<Task> DoDuplicate() const override;
		Size GetOperationsPerformedPerRun() override;
		Duration GetExecutionMinimumDuration() override;
	private:
		UniquePtr<AbstractTaskData> _outputData;
		UniquePtr<AbstractTaskData> _inputData;
		XmlParser _parser;
		XmlElement _element;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVGPARSETASK_HPP
