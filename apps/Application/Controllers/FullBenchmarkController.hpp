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

	class FullBenchmarkController : public Controller<FullBenchmarkModel>
	{
	public:
		void RunAsync();
		void StopRunning();
		void ClearResults();
		void SaveResults(const std::filesystem::path& filePath);
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
		MessageService& _messageService;
		std::thread _runnerThread;
		std::atomic<bool> _running;
		std::atomic<bool> _cancelling;

		std::vector<std::unique_ptr<FullBenchmarkInstance>> _benchmarks;
	};

} // Elpida
// Application

#endif //ELPIDA_FULLBENCHMARKCONTROLLER_HPP
