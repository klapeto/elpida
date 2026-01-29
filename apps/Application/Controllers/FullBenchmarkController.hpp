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
// Created by klapeto on 22/10/2023.
//

#ifndef ELPIDA_FULLBENCHMARKCONTROLLER_HPP
#define ELPIDA_FULLBENCHMARKCONTROLLER_HPP

#include "Models/Full/FullBenchmarkModel.hpp"
#include "Controller.hpp"
#include "FullBenchmarkInstance.hpp"

#include <thread>
#include <atomic>
#include <filesystem>
#include <vector>
#include <memory>

namespace Elpida::Application
{
	class BenchmarkExecutionService;
	class TimingModel;
	class ResultSerializer;
	class MessageService;
	class BenchmarkRunConfigurationModel;
	class TopologyModel;
	class MemoryInfoModel;
	class ResultsHTMLReporter;
	class PathsService;
	class DesktopService;
	class DataUploader;

	class FullBenchmarkController : public Controller<FullBenchmarkModel>
	{
	public:
		void RunAsync();
		void StopRunning();
		void ClearResults();
		void SaveResults(const std::filesystem::path& filePath);

		void WaitToCompleteRun();

		[[nodiscard]]
		const std::vector<std::string>& GetMissingBenchmarks() const;

		[[nodiscard]]
		const std::vector<std::unique_ptr<FullBenchmarkInstance>> & GetBenchmarks() const;

		FullBenchmarkController(FullBenchmarkModel& model,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				const MemoryInfoModel& memoryInfoModel,
				const BenchmarkRunConfigurationModel& runConfigurationModel,
				BenchmarkExecutionService& benchmarkExecutionService,
				const ResultSerializer& resultSerializer,
				const ResultsHTMLReporter& resultsHTMLReporter,
				const PathsService& pathsService,
				const DesktopService& desktopService,
				const DataUploader& dataUploader,
				MessageService& messageService,
				const std::vector<BenchmarkGroupModel>& benchmarkGroups);

		~FullBenchmarkController() override;
	private:
		const TimingModel& _timingModel;
		const TopologyModel& _topologyModel;
		const BenchmarkRunConfigurationModel& _runConfigurationModel;
		BenchmarkExecutionService& _benchmarkExecutionService;
		const ResultSerializer& _resultSerializer;
		const ResultsHTMLReporter& _resultsHTMLReporter;
		const PathsService& _pathsService;
		const DesktopService& _desktopService;
		const DataUploader& _dataUploader;
		MessageService& _messageService;
		std::thread _runnerThread;
		std::atomic<bool> _running;
		std::atomic<bool> _cancelling;
		std::vector<std::string> _missingBenchmarks;

		std::vector<std::unique_ptr<FullBenchmarkInstance>> _benchmarks;
		void GenerateHtmlReport(const std::vector<FullBenchmarkResultModel>& thisResults, Duration duration) const;
		void PostHandleResults(const std::vector<FullBenchmarkResultModel>& thisResults, Duration duration) const;
		static Score CalculateTotalScore(Score singleCoreScore, Score multiCoreScore);
	};

} // Elpida
// Application

#endif //ELPIDA_FULLBENCHMARKCONTROLLER_HPP
