//
// Created by klapeto on 1/3/2023.
//

#ifndef _PNGENCODINGTASK_HPP_
#define _PNGENCODINGTASK_HPP_

#include <optional>
#include <png.h>

#include "Elpida/Task.hpp"

namespace Elpida
{

	class PngEncodingTask final: public Task
	{
	 public:
		void Prepare(TaskData&& inputData) override;
		TaskData Finalize() override;
		TaskInfo GetInfo() const override;
		bool CanBeMultiThreaded() const override;

		PngEncodingTask();
		~PngEncodingTask() override;
	 protected:
		void DoRun() override;
		std::unique_ptr<Task> DoDuplicate() const override;
	 private:
		std::unique_ptr<TaskData> _outputData;
		std::optional<TaskData> _inputData;
		png_image _pngImg;
		unsigned int _width;
		unsigned int _height;
	};

} // Elpida

#endif //_PNGENCODINGTASK_HPP_
