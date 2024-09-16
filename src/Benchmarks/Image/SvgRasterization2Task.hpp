//
// Created by klapeto on 7/9/2024.
//

#ifndef ELPIDA_SVGRASTERIZATION2TASK_HPP
#define ELPIDA_SVGRASTERIZATION2TASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"
#include "Elpida/Svg/SvgSuperSampler.hpp"
#include "Elpida/Svg/SvgBackDrop.hpp"

namespace Elpida
{

	class SvgRasterization2Task: public MicroTask
	{
	public:

		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		explicit SvgRasterization2Task(std::size_t subSamples, bool multiThread);
		~SvgRasterization2Task() override = default;
	protected:
		void DoRun(Iterations iterations) override;

		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		SharedPtr<AbstractTaskData> _inputData;
		SvgCalculatedDocument* _inputDocument;
		SvgBackDrop _rasterizedImage;
		SvgSuperSampler _superSampler;
		bool _multiThread;
	};

} // Elpida

#endif //ELPIDA_SVGRASTERIZATION2TASK_HPP
