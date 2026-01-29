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
// Created by klapeto on 16/4/2023.
//

#ifndef ELPIDA_CUSTOMBENCHMARKSCONTROLLER_HPP_
#define ELPIDA_CUSTOMBENCHMARKSCONTROLLER_HPP_

#include "Controller.hpp"
#include "Models/Custom/CustomBenchmarkModel.hpp"
#include <filesystem>

namespace Elpida::Application
{
	class BenchmarkModel;
	class TopologyModel;
	class MessageService;
	class BenchmarkExecutionService;
	class BenchmarkRunConfigurationModel;
	class TimingModel;
	class ResultSerializer;
	class ResultsHTMLReporter;
	class PathsService;
	class DesktopService;

	class CustomBenchmarkController : public Controller<CustomBenchmarkModel>
	{
	 public:

		void SetCurrentBenchmark(const BenchmarkModel* currentBenchmark);

		void Run();
		void StopRunning();
		void ClearResults();
		void SaveResults(const std::filesystem::path& filePath);
		void GenerateReport(const std::filesystem::path& filePath);

		explicit CustomBenchmarkController(CustomBenchmarkModel& model,
				TopologyModel& topologyModel, TimingModel& overheadsModel,
				BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
				BenchmarkExecutionService& benchmarkExecutionService, const ResultSerializer& resultSerializer,
				const ResultsHTMLReporter& resultsHtmlReporter, const PathsService& pathsService,
				const DesktopService& desktopService,
				MessageService& messageService);
		~CustomBenchmarkController() override = default;
	 private:
		TopologyModel& _topologyModel;
		TimingModel& _overheadsModel;
		BenchmarkRunConfigurationModel& _benchmarkRunConfigurationModel;
		BenchmarkExecutionService& _benchmarkExecutionService;
		const ResultSerializer& _resultSerializer;
		const ResultsHTMLReporter& _resultsHTMLReporter;
		const PathsService& _pathsService;
		const DesktopService& _desktopService;
		MessageService& _messageService;
		bool _cancelling;
		void GenerateHtmlReport(const BenchmarkModel* selectedBenchmark, const std::vector<std::size_t>& affinity,
				const std::vector<BenchmarkResultModel>& thisRunResults) const;
	};

} // Application

#endif //ELPIDA_CUSTOMBENCHMARKSCONTROLLER_HPP_
