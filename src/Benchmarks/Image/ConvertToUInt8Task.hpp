//
// Created by klapeto on 5/3/2023.
//

#ifndef _CONVERTTOUINT8TASK_HPP_
#define _CONVERTTOUINT8TASK_HPP_

#include <cstdint>
#include <memory>
#include <optional>

#include "Elpida/Task.hpp"
#include "ImageBenchmarksConfig.hpp"

namespace Elpida
{

	class ConvertToUInt8Task: public Task
	{
	 public:
		void Prepare(TaskData&& inputData) override;
		TaskData Finalize() override;
		TaskInfo GetInfo() const override;
		bool CanBeMultiThreaded() const override;

		ConvertToUInt8Task();
		~ConvertToUInt8Task() override = default;
	 protected:
		void DoRun() override;
		std::unique_ptr<Task> DoDuplicate() const override;
	 private:
		std::unique_ptr<TaskData> _outputData;
		std::optional<TaskData> _inputData;
		std::size_t _width;
		std::size_t _height;
		std::size_t _channels;
		std::size_t _inputBytesPerChannel;
	};

} // Elpida

#endif //_CONVERTTOUINT8TASK_HPP_
