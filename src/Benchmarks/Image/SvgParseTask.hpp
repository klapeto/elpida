//
// Created by klapeto on 31/10/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVGPARSETASK_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVGPARSETASK_HPP

#include "Elpida/Core/Task.hpp"

#include "nanosvg.h"

namespace Elpida
{

	class SvgParseTask final: public Task
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
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;
	private:
		NSVGimage* _image{nullptr};
		UniquePtr<AbstractTaskData> _outputData;
		UniquePtr<AbstractTaskData> _inputData;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVGPARSETASK_HPP
