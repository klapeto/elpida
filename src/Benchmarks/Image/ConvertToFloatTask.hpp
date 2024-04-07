//
// Created by klapeto on 5/3/2023.
//

#ifndef _CONVERTTOFLOATTASK_HPP_
#define _CONVERTTOFLOATTASK_HPP_


#include "Elpida/Core/Task.hpp"
#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/UniquePtr.hpp"

#include "ImageTaskData.hpp"
#include "ImageBenchmarksConfig.hpp"

namespace Elpida
{

	class ConvertToFloatTask : public Task
	{
	 public:
		void Prepare(UniquePtr<AbstractTaskData> inputData) override;

		UniquePtr<AbstractTaskData> Finalize() override;
		bool CanBeMultiThreaded() const override;
		Size GetProcessedDataSize() const override;

		ConvertToFloatTask();
		~ConvertToFloatTask() override = default;
	 protected:
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;
		TaskInfo DoGetInfo() const override;
	 private:
		UniquePtr<ImageTaskData> _outputData;
		UniquePtr<AbstractTaskData> _inputData;
		IntChannel* _inPtr;
		FloatChannel* _outPtr;
		Size _sizeInChannels;
		unsigned int _channels;
	};

} // Elpida

#endif //_CONVERTTOFLOATTASK_HPP_
