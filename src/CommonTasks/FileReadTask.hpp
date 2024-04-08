//
// Created by klapeto on 2/3/2023.
//

#ifndef ELPIDA_COMMONTASKS_FILEREADTASK_HPP_
#define ELPIDA_COMMONTASKS_FILEREADTASK_HPP_


#include <fstream>

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Task.hpp"

namespace Elpida
{

	class FileReadTask : public Task
	{
	 public:
		void Prepare(UniquePtr<AbstractTaskData> inputData) override;
		UniquePtr<AbstractTaskData> Finalize() override;
		bool CanBeMultiThreaded() const override;
		Size GetProcessedDataSize() const override;

		explicit FileReadTask(std::string filePath);
		~FileReadTask() override = default;
	 protected:
		void DoRun() override;
		std::unique_ptr<Task> DoDuplicate() const override;
		TaskInfo DoGetInfo() const override;
	 private:
		UniquePtr<AbstractTaskData> _outputData;
		std::fstream _fileStream;
		std::string _filePath;
	};

} // Elpida

#endif //ELPIDA_COMMONTASKS_FILEREADTASK_HPP_
