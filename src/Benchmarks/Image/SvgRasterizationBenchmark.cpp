/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
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
// Created by klapeto on 9/4/2024.
//

#include "SvgRasterizationBenchmark.hpp"

#include "CommonTasks/ParseXmlTask.hpp"
#include "SvgParseTask.hpp"
#include "SvgCalculateTask.hpp"
#include "SvgDocumentGenerationTask.hpp"
#include "SvgRasterizationTask.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"

namespace Elpida
{
	std::vector<TaskConfiguration> SvgRasterizationBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Shape count", ConfigurationType::Integer, "1"),
				TaskConfiguration("SVG output scale", ConfigurationType::Float, "1.0"),
				TaskConfiguration("SVG rasterization sub samples", ConfigurationType::Integer, "16")
		};
	}

	std::vector<std::unique_ptr<Task>> SvgRasterizationBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<SvgDocumentGenerationTask>(configuration.at(0).AsInteger()));
		returnTasks.push_back(CreateTask<ParseXmlTask>());
		returnTasks.push_back(CreateTask<SvgParseTask>());
		returnTasks.push_back(CreateTask<SvgCalculateTask>(configuration.at(1).AsFloat()));
		returnTasks.push_back(CreateTask<SvgRasterizationTask>(configuration.at(2).AsInteger()));

		return returnTasks;
	}

	void SvgRasterizationBenchmark::DoGetBenchmarkInfo(
			String& name,
			String& description,
			size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "Svg Rasterization";
		description = "Rasterizes a calculated Svg document.";
		taskToUseAsScoreIndex = 4;

		taskInfos.push_back(SvgDocumentGenerationTask(1).GetInfo());
		taskInfos.push_back(ParseXmlTask().GetInfo());
		taskInfos.push_back(SvgParseTask().GetInfo());
		taskInfos.push_back(SvgCalculateTask(1.0).GetInfo());
		taskInfos.push_back(SvgRasterizationTask(16).GetInfo());
	}
} // Elpida