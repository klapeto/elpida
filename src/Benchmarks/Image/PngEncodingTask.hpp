//
// Created by klapeto on 1/3/2023.
//

#ifndef _PNGENCODINGTASK_HPP_
#define _PNGENCODINGTASK_HPP_

#include <png.h>

#include "Elpida/UniquePtr.hpp"
#include "Elpida/Task.hpp"

namespace Elpida
{

	class PngEncodingTask final : public Task
	{
	 public:
		void Prepare(UniquePtr<AbstractTaskData> inputData) override;
		UniquePtr<AbstractTaskData> Finalize() override;
		TaskInfo GetInfo() const override;
		bool CanBeMultiThreaded() const override;

		PngEncodingTask();
		~PngEncodingTask() override;
	 protected:
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;
	 private:
		UniquePtr<AbstractTaskData> _outputData;
		UniquePtr<AbstractTaskData> _inputData;
		png_image _pngImg;
		unsigned int _width;
		unsigned int _height;
	};

} // Elpida

#endif //_PNGENCODINGTASK_HPP_
