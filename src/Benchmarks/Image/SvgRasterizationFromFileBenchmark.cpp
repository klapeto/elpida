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
// Created by klapeto on 16/7/2024.
//

#include "SvgRasterizationFromFileBenchmark.hpp"

#include "CommonTasks/ParseXmlTask.hpp"
#include "SvgParseTask.hpp"
#include "SvgCalculateTask.hpp"
#include "SvgDocumentGenerationTask.hpp"
#include "SvgRasterizationTask.hpp"
#include "ConvertToUInt8Task.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/FileReadTask.hpp"
#include "CommonTasks/FileWriteTask.hpp"
#include "PngEncodingTask.hpp"

namespace Elpida
{
	std::vector<TaskConfiguration> SvgRasterizationFromFileBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Input file", ConfigurationType::File, ""),
				TaskConfiguration("SVG output scale", ConfigurationType::Float, "1.0"),
				TaskConfiguration("SVG rasterization sub samples", ConfigurationType::Integer, "16"),
				TaskConfiguration("Output Png file", ConfigurationType::File, "./test-data-single.png"),
		};
	}

	std::vector<std::unique_ptr<Task>> SvgRasterizationFromFileBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<FileReadTask>(configuration.at(0).GetValue()));
		returnTasks.push_back(CreateTask<ParseXmlTask>());
		returnTasks.push_back(CreateTask<SvgParseTask>());
		returnTasks.push_back(CreateTask<SvgCalculateTask>(configuration.at(1).AsFloat()));
		returnTasks.push_back(CreateTask<SvgRasterizationTask>(configuration.at(2).AsInteger()));
		returnTasks.push_back(CreateTask<ConvertToUInt8Task>());
		returnTasks.push_back(CreateTask<PngEncodingTask>());
		returnTasks.push_back(CreateTask<FileWriteTask>(configuration.at(3).GetValue()));

		return returnTasks;
	}

	void SvgRasterizationFromFileBenchmark::DoGetBenchmarkInfo(String& name, String& description,
			size_t& taskToUseAsScoreIndex, std::vector<TaskInfo>& taskInfos) const
	{
		name = "Svg Rasterization from file";
		description = "Rasterizes a calculated Svg document from the disk.";
		taskToUseAsScoreIndex = 4;

		taskInfos.push_back(FileReadTask("").GetInfo());
		taskInfos.push_back(ParseXmlTask().GetInfo());
		taskInfos.push_back(SvgParseTask().GetInfo());
		taskInfos.push_back(SvgCalculateTask(1.0).GetInfo());
		taskInfos.push_back(SvgRasterizationTask(16).GetInfo());
		taskInfos.push_back(ConvertToUInt8Task().GetInfo());
		taskInfos.push_back(PngEncodingTask().GetInfo());
		taskInfos.push_back(FileWriteTask("").GetInfo());
	}
} // Elpida