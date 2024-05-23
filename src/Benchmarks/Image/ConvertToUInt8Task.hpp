//
// Created by klapeto on 5/3/2023.
//

#ifndef _CONVERTTOUINT8TASK_HPP_
#define _CONVERTTOUINT8TASK_HPP_

#include <cstdint>
#include <memory>
#include <optional>

#include "Elpida/Core/Task.hpp"
#include "ImageBenchmarksConfig.hpp"

namespace Elpida
{

	class ConvertToUInt8Task: public Task
	{
	 public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;

		ConvertToUInt8Task();
		~ConvertToUInt8Task() override = default;
	 protected:
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;
		TaskInfo DoGetInfo() const override;
	 private:
		SharedPtr<AbstractTaskData> _outputData;
		SharedPtr<AbstractTaskData> _inputData;
		IntChannel* _outPtr;
		FloatChannel* _inPtr;
		Size _sizeInChannels;
		unsigned int _channels;
	};

} // Elpida

#endif //_CONVERTTOUINT8TASK_HPP_
