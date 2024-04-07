//
// Created by klapeto on 2/3/2023.
//

#ifndef _PNGDECODINGTASK_HPP_
#define _PNGDECODINGTASK_HPP_

#include <png.h>

#include "Elpida/Core/Task.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "ImageTaskData.hpp"

namespace Elpida
{

	class PngDecodingTask final : public Task
	{
	 public:
		void Prepare(UniquePtr<AbstractTaskData> inputData) override;
		UniquePtr<AbstractTaskData> Finalize() override;
		bool CanBeMultiThreaded() const override;
		Size GetProcessedDataSize() const override;

		PngDecodingTask();
		~PngDecodingTask() override;
	 protected:
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;
		TaskInfo DoGetInfo() const override;
	 private:
		UniquePtr<ImageTaskData> _outputData;
		UniquePtr<AbstractTaskData> _inputData;
		png_image _pngImg;
	};

} // Elpida

#endif //_PNGDECODINGTASK_HPP_
