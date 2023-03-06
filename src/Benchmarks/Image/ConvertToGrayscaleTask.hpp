//
// Created by klapeto on 5/3/2023.
//

#ifndef _CONVERTTOGRAYSCALETASK_HPP_
#define _CONVERTTOGRAYSCALETASK_HPP_

#include <cstdint>
#include <memory>
#include <optional>

#include "Elpida/Task.hpp"
#include "ImageBenchmarksConfig.hpp"

namespace Elpida
{

	class ConvertToGrayscaleTask : public Task
	{
	 public:
		void Prepare(TaskData&& inputData) override;
		TaskData Finalize() override;
		TaskInfo GetInfo() const override;
		bool CanBeMultiThreaded() const override;

		ConvertToGrayscaleTask();
		~ConvertToGrayscaleTask() override = default;
	 protected:
		void DoRun() override;
		std::unique_ptr<Task> DoDuplicate() const override;

	 private:
		std::optional<TaskData> _inputData;
		std::size_t _width;
		std::size_t _height;
		std::size_t _channels;
	};

} // Elpida

#endif //_CONVERTTOGRAYSCALETASK_HPP_
