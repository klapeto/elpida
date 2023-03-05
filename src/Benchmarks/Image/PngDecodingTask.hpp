//
// Created by klapeto on 2/3/2023.
//

#ifndef _PNGDECODINGTASK_HPP_
#define _PNGDECODINGTASK_HPP_

#include <png.h>
#include <memory>
#include <optional>

#include "Elpida/Task.hpp"

namespace Elpida
{

	class PngDecodingTask final : public Task
	{
	 public:
		void Prepare(TaskData&& inputData) override;
		TaskData Finalize() override;
		TaskInfo GetInfo() const override;
		bool CanBeMultiThreaded() const override;

		PngDecodingTask();
		~PngDecodingTask() override;
	 protected:
		void DoRun() override;
		std::unique_ptr<Task> DoDuplicate() const override;
	 private:
		std::unique_ptr<TaskData> _outputData;
		std::optional<TaskData> _inputData;
		png_image _pngImg;
	};

} // Elpida

#endif //_PNGDECODINGTASK_HPP_
