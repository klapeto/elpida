//
// Created by klapeto on 9/4/2024.
//

#ifndef ELPIDA_SVGRASTERIZATIONTASK_HPP
#define ELPIDA_SVGRASTERIZATIONTASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"
#include "Elpida/Svg/SvgBackDrop.hpp"

namespace Elpida
{

	class ThreadPool;

	class SvgRasterizationTask: public MicroTask
	{
	public:

		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		explicit SvgRasterizationTask(std::size_t subSamples);
		~SvgRasterizationTask() override = default;
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
		SharedPtr<AbstractTaskData> _inputData;
		SvgCalculatedDocument _inputDocument;
		SvgBackDrop _rasterizedImage;
		std::size_t _subSamples;
	};

} // Elpida

#endif //ELPIDA_SVGRASTERIZATIONTASK_HPP
