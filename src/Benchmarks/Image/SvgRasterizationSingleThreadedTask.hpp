//
// Created by klapeto on 8/4/2024.
//

#ifndef ELPIDA_SVGRASTERIZATIONSINGLETHREADEDTASK_HPP
#define ELPIDA_SVGRASTERIZATIONSINGLETHREADEDTASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"
#include "Elpida/Svg/SvgBackDrop.hpp"

namespace Elpida
{

	class SvgRasterizationSingleThreadedTask : public MicroTask
	{
	public:

		void Prepare(UniquePtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		bool CanBeMultiThreaded() const override;

		[[nodiscard]]
		UniquePtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		explicit SvgRasterizationSingleThreadedTask(std::size_t subSamples);
		~SvgRasterizationSingleThreadedTask() override = default;
	private:
		std::size_t _subSamples;
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
		SvgCalculatedDocument _inputDocument;
		SvgBackDrop _rasterizedImage;
	};

} // Elpida

#endif //ELPIDA_SVGRASTERIZATIONSINGLETHREADEDTASK_HPP
