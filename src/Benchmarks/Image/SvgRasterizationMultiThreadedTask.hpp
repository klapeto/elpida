//
// Created by klapeto on 9/4/2024.
//

#ifndef ELPIDA_SVGRASTERIZATIONMULTITHREADEDTASK_HPP
#define ELPIDA_SVGRASTERIZATIONMULTITHREADEDTASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"
#include "Elpida/Svg/SvgBackDrop.hpp"

namespace Elpida
{

	class ThreadPool;

	class SvgRasterizationMultiThreadedTask: public MicroTask
	{
	public:

		void Prepare(UniquePtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		bool CanBeMultiThreaded() const override;

		[[nodiscard]]
		UniquePtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		explicit SvgRasterizationMultiThreadedTask(std::size_t subSamples, ThreadPool* threadPool);
		~SvgRasterizationMultiThreadedTask() override = default;
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
		ThreadPool* _threadPool;
		std::size_t _subSamples;
	};

} // Elpida

#endif //ELPIDA_SVGRASTERIZATIONMULTITHREADEDTASK_HPP
