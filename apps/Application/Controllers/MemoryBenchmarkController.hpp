//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 18/6/2024.
//

#ifndef ELPIDA_MEMORYBENCHMARKCONTROLLER_HPP
#define ELPIDA_MEMORYBENCHMARKCONTROLLER_HPP

#include "Controller.hpp"
#include "Models/MemoryBenchmark/MemoryBenchmarkModel.hpp"
#include "Models/Benchmark/BenchmarkGroupModel.hpp"
#include "Models/Benchmark/BenchmarkResultModel.hpp"
#include <thread>
#include <atomic>
#include <filesystem>

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
	class BenchmarkModel;
	class SettingsService;

	class MemoryBenchmarkController : public Controller<MemoryBenchmarkModel>
	{
	public:
		void StopRunning();
		void RunAsync();

		void SaveResults(const std::filesystem::path& filePath);
		void ClearResults();

		void SetMultiplier(double multiplier);
		void SetInitialBytes(size_t initialBytes);
		void SetIterations(size_t iterations);
		void SetBenchmarkType(MemoryBenchmarkType type);

		explicit MemoryBenchmarkController(MemoryBenchmarkModel& model,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				const MemoryInfoModel& memoryInfoModel,
				const BenchmarkRunConfigurationModel& runConfigurationModel,
				BenchmarkExecutionService& benchmarkExecutionService,
				const ResultSerializer& resultSerializer,
				const ResultsHTMLReporter& resultsHTMLReporter,
				const PathsService& pathsService,
				const DesktopService& desktopService,
				MessageService& messageService,
				SettingsService& settingsService,
				const std::vector<BenchmarkGroupModel>& benchmarkGroups);
		~MemoryBenchmarkController() override;
	private:
		const TimingModel& _timingModel;
		const TopologyModel& _topologyModel;
		const BenchmarkRunConfigurationModel& _runConfigurationModel;
		BenchmarkExecutionService& _benchmarkExecutionService;
		const ResultSerializer& _resultSerializer;
		const ResultsHTMLReporter& _resultsHTMLReporter;
		const PathsService& _pathsService;
		const DesktopService& _desktopService;
		SettingsService& _settingsService;
		MessageService& _messageService;
		std::thread _runnerThread;
		std::atomic<bool> _running;
		std::atomic<bool> _cancelling;
		std::vector<std::size_t> _targetProcessors;
		std::vector<std::size_t> _targetSizes;

		const BenchmarkModel* _memoryLatencyBenchmark;
		const BenchmarkModel* _memoryReadBandwidthBenchmark;

		void CalculateTargetSizesAndProcessors();
		void GenerateHtmlReport(const std::vector<MemoryBenchmarkResultModel>& thisResults) const;
		BenchmarkResultModel RunBenchmark(const BenchmarkModel* benchmark, std::size_t size) const;
		const BenchmarkModel* GetBenchmarkToRun() const;
	};

} // Application
// Elpida

#endif //ELPIDA_MEMORYBENCHMARKCONTROLLER_HPP
