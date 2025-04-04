//
// Created by klapeto on 9/4/2024.
//

#ifndef ELPIDA_SVGRASTERIZATIONTASK_HPP
#define ELPIDA_SVGRASTERIZATIONTASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"
#include "Elpida/Svg/SvgSuperSampler.hpp"
#include "Elpida/Svg/SvgBackDrop.hpp"

namespace Elpida
{
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
		void DoRunImpl() override;

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
	};

} // Elpida

#endif //ELPIDA_SVGRASTERIZATIONTASK_HPP
