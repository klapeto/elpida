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
				const DesktopService& desktopService);
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
		bool _cancelling;
	};

} // Application

#endif //ELPIDA_CUSTOMBENCHMARKSCONTROLLER_HPP_
