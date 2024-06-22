//
// Created by klapeto on 18/6/2024.
//

#ifndef ELPIDA_MEMORYOVERHEADCALCULATIONCONTROLLER_HPP
#define ELPIDA_MEMORYOVERHEADCALCULATIONCONTROLLER_HPP

#include "Controller.hpp"
#include "Models/MemoryOverhead/MemoryOverheadCalculationModel.hpp"
#include "Models/Benchmark/BenchmarkGroupModel.hpp"
#include <thread>
#include <atomic>

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

	class MemoryOverheadCalculationController : public Controller<MemoryOverheadCalculationModel>
	{
	public:
		void StopRunning();
		void RunAsync();

		void SetSubSamplesMultiplier(double subSamplesMultiplier);
		void SetInitialSubSamples(size_t initialSubSamples);
		void SetScaleMultiplier(double scaleMultiplier);
		void SetInitialScale(double initialScale);
		void SetIterations(size_t iterations);

		explicit MemoryOverheadCalculationController(MemoryOverheadCalculationModel& model,
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
		~MemoryOverheadCalculationController();
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
		std::vector<double> _targetScales;
		std::vector<std::size_t> _targetSubSamples;
		std::vector<std::size_t> _targetProcessors;

		const BenchmarkModel* _benchmark;

		void CalculateTargetScales();
		void GenerateHtmlReport(const std::vector<MemoryOverheadResultModel>& thisResults) const;
	};

} // Application
// Elpida

#endif //ELPIDA_MEMORYOVERHEADCALCULATIONCONTROLLER_HPP
