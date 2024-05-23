//
// Created by klapeto on 1/3/2023.
//

#ifndef _PNGENCODINGTASK_HPP_
#define _PNGENCODINGTASK_HPP_

#include <png.h>

#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Task.hpp"

namespace Elpida
{

	class PngEncodingTask final : public Task
	{
	 public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;

		PngEncodingTask();
		~PngEncodingTask() override;
	 protected:
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;
		TaskInfo DoGetInfo() const override;
	 private:
		SharedPtr<AbstractTaskData> _outputData;
		SharedPtr<AbstractTaskData> _inputData;
		png_image _pngImg;
		unsigned int _width;
		unsigned int _height;
	};

} // Elpida

#endif //_PNGENCODINGTASK_HPP_
