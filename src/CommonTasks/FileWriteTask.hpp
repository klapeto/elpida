/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		ConcurrencyMode GetAllowedConcurrency() const override;
		Size GetProcessedDataSize() const override;

		explicit FileWriteTask(std::string filePath);
		~FileWriteTask() override = default;
	 protected:
		void DoRun() override;
		std::unique_ptr<Task> DoDuplicate() const override;
		TaskInfo DoGetInfo() const override;
	 private:
		SharedPtr<AbstractTaskData> _inputData;
		std::fstream _fileStream;
		std::string _filePath;
	};

} // Elpida

#endif //ELPIDA_COMMONTASKS_FILEWRITETASK_HPP_
