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

		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		SvgParseTask() = default;
		~SvgParseTask() override = default;
	protected:
		void DoRunImpl() override;

		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		SharedPtr<AbstractTaskData> _inputData;
		XmlElement _inputXmlElement;
		SvgDocument _parsedElement;
	};

} // Elpida

#endif //ELPIDA_SVGPARSETASK_HPP
