//
// Created by klapeto on 31/10/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVGRASTERIZATION_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVGRASTERIZATION_HPP

#include "Elpida/Core/Task.hpp"

#include "nanosvg.h"
#include "nanosvgrast.h"

namespace Elpida
{

	class SvgRasterization final : public Task
	{
	public:
		void Prepare(UniquePtr<AbstractTaskData> inputData) override;
		UniquePtr<AbstractTaskData> Finalize() override;
		TaskInfo GetInfo() const override;
		bool CanBeMultiThreaded() const override;
		Size GetProcessedDataSize() const override;

		SvgRasterization(float scale);
		~SvgRasterization() override;
	protected:
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;
	private:
		UniquePtr<AbstractTaskData> _outputData;
		UniquePtr<AbstractTaskData> _imageData;
		NSVGimage* _image;
		NSVGrasterizer* _rasterizer;
		float _scale;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVGRASTERIZATION_HPP
