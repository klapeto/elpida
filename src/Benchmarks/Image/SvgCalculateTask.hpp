//
// Created by klapeto on 7/4/2024.
//

#ifndef ELPIDA_SVGCALCULATETASK_HPP
#define ELPIDA_SVGCALCULATETASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Svg/SvgDocument.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"

namespace Elpida
{

	class SvgCalculateTask: public MicroTask
	{
	public:

		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		explicit SvgCalculateTask(double scale);
		~SvgCalculateTask() override = default;
	private:
		double _scale;
	protected:
		void DoRunImpl() override;

		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		SharedPtr<AbstractTaskData> _inputData;
		SvgDocument _inputDocument;
		SvgCalculatedDocument _calculatedDocument;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATETASK_HPP
