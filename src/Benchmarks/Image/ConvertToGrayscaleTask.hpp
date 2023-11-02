//
// Created by klapeto on 5/3/2023.
//

#ifndef _CONVERTTOGRAYSCALETASK_HPP_
#define _CONVERTTOGRAYSCALETASK_HPP_

#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Task.hpp"

#include "ImageBenchmarksConfig.hpp"

namespace Elpida
{

	class ConvertToGrayscaleTask : public Task
	{
	 public:
		void Prepare(UniquePtr<AbstractTaskData> inputData) override;
		UniquePtr<AbstractTaskData> Finalize() override;
		TaskInfo GetInfo() const override;
		bool CanBeMultiThreaded() const override;
		Size GetProcessedDataSize() const override;

		ConvertToGrayscaleTask();
		~ConvertToGrayscaleTask() override = default;
	 protected:
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;

	 private:
		UniquePtr<AbstractTaskData> _inputData;
		FloatChannel* _inPtr;
		Size _sizeInChannels;
		unsigned int _channels;
	};

} // Elpida

#endif //_CONVERTTOGRAYSCALETASK_HPP_
