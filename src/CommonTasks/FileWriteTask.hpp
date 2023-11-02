//
// Created by klapeto on 2/3/2023.
//

#ifndef ELPIDA_COMMONTASKS_FILEWRITETASK_HPP_
#define ELPIDA_COMMONTASKS_FILEWRITETASK_HPP_

#include <fstream>

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/Task.hpp"

namespace Elpida
{

	class FileWriteTask final : public Task
	{
	 public:
		void Prepare(UniquePtr<AbstractTaskData> inputData) override;
		UniquePtr<AbstractTaskData> Finalize() override;
		TaskInfo GetInfo() const override;
		bool CanBeMultiThreaded() const override;
		Size GetProcessedDataSize() const override;

		explicit FileWriteTask(std::string filePath);
		~FileWriteTask() override = default;
	 protected:
		void DoRun() override;
		std::unique_ptr<Task> DoDuplicate() const override;
	 private:
		UniquePtr<AbstractTaskData> _inputData;
		std::fstream _fileStream;
		std::string _filePath;
	};

} // Elpida

#endif //ELPIDA_COMMONTASKS_FILEWRITETASK_HPP_
