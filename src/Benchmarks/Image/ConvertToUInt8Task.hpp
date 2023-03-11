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
		void Prepare(UniquePtr<AbstractTaskData> inputData) override;
		UniquePtr<AbstractTaskData> Finalize() override;
		TaskInfo GetInfo() const override;
		bool CanBeMultiThreaded() const override;

		ConvertToUInt8Task();
		~ConvertToUInt8Task() override = default;
	 protected:
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;
	 private:
		UniquePtr<AbstractTaskData> _outputData;
		UniquePtr<AbstractTaskData> _inputData;
		IntChannel* _outPtr;
		FloatChannel* _inPtr;
		Size _sizeInChannels;
		unsigned int _channels;
	};

} // Elpida

#endif //_CONVERTTOUINT8TASK_HPP_
