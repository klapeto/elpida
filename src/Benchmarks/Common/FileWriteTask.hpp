//
// Created by klapeto on 2/3/2023.
//

#ifndef _FILEWRITETASK_HPP_
#define _FILEWRITETASK_HPP_

#include <string>
#include <fstream>

#include "Elpida/Task.hpp"

namespace Elpida
{

	class FileWriteTask final : public Task
	{
	 public:
		void Prepare(TaskData&& inputData) override;
		TaskData Finalize() override;
		TaskInfo GetInfo() const override;
		bool CanBeMultiThreaded() const override;

		explicit FileWriteTask(std::string filePath);
		~FileWriteTask() override = default;
	 protected:
		void DoRun() override;
		std::unique_ptr<Task> DoDuplicate() const override;
	 private:
		TaskData _inputData;
		std::fstream _fileStream;
		std::string _filePath;
	};

} // Elpida

#endif //_FILEWRITETASK_HPP_
