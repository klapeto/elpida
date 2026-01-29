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

#include "PngEncodingBenchmark.hpp"

#include "CommonTasks/FileReadTask.hpp"
#include "CommonTasks/FileWriteTask.hpp"
#include "PngDecodingTask.hpp"
#include "PngEncodingTask.hpp"
#include "ConvertToFloatTask.hpp"
#include "ConvertToGrayscaleTask.hpp"
#include "ConvertToUInt8Task.hpp"
#include "Elpida/Core/TaskConfiguration.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"

namespace Elpida
{
	std::vector<std::unique_ptr<Task>> PngEncodingBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(std::make_unique<FileReadTask>(configuration.at(0).GetValue()));
		returnTasks.push_back(std::make_unique<PngDecodingTask>());
		returnTasks.push_back(std::make_unique<PngEncodingTask>());
		returnTasks.push_back(std::make_unique<FileWriteTask>(configuration.at(1).GetValue()));

		return returnTasks;
	}

	std::vector<TaskConfiguration> PngEncodingBenchmark::GetRequiredConfiguration() const
	{
		return {
			TaskConfiguration("Input PNG file", ConfigurationType::File, "./testImage.png"),
			TaskConfiguration("Output PNG file", ConfigurationType::File, "./testImage.out.png"),
		};
	}

	void PngEncodingBenchmark::DoGetBenchmarkInfo(String& name, String& description,
			size_t& coreTaskIndex, std::vector<TaskInfo>& taskInfos) const
	{
		name = "PngEncoding";
		description = "Encodes an image file to PNG.";
		coreTaskIndex = 2;

		taskInfos.push_back(FileReadTask("").GetInfo());
		taskInfos.push_back(PngDecodingTask().GetInfo());
		taskInfos.push_back(PngEncodingTask().GetInfo());
		taskInfos.push_back(FileWriteTask("").GetInfo());

	}
} // Elpida