//
// Created by klapeto on 22/10/2023.
//

#include "FullBenchmarkController.hpp"

#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"

#include "Core/BenchmarkExecutionService.hpp"
#include "Core/MessageService.hpp"
#include "Core/ResultsHTMLReporter.hpp"
#include "Core/PathsService.hpp"
#include "Core/DesktopService.hpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "ResultSerializer.hpp"

#include "FullBenchmarkInstances/SvgRasterizationSingleThread.hpp"
#include "FullBenchmarkInstances/SvgRasterizationMultiThread.hpp"
#include "FullBenchmarkInstances/MemoryLatency.hpp"
#include "FullBenchmarkInstances/MemoryReadBandwidth.hpp"

#include <sstream>
#include <fstream>
#include <locale>
#include <string>

namespace Elpida::Application
{
	FullBenchmarkController::FullBenchmarkController(FullBenchmarkModel& model,
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
			const std::vector<BenchmarkGroupModel>& benchmarkGroups)
			:
			Controller(model),
			_timingModel(timingModel),
			_topologyModel(topologyModel),
			_runConfigurationModel(runConfigurationModel),
			_benchmarkExecutionService(benchmarkExecutionService),
			_resultSerializer(resultSerializer),
			_resultsHTMLReporter(resultsHTMLReporter),
			_pathsService(pathsService),
			_desktopService(desktopService),
			_messageService(messageService),
			_running(false),
			_cancelling(false)
	{
		for (auto& group : benchmarkGroups)
		{
			for (auto& benchmark : group.GetBenchmarks())
			{
				auto& name = benchmark.GetName();
				if (name == "Memory latency")
				{
					_benchmarks.push_back(
							std::make_unique<MemoryLatency>(benchmark, timingModel, topologyModel, memoryInfoModel,
									benchmarkExecutionService));
				}
				else if (name == "Memory read bandwidth")
				{
					_benchmarks.push_back(std::make_unique<MemoryReadBandwidth>(benchmark, timingModel, topologyModel,
							memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Svg Rasterization")
				{
					_benchmarks.push_back(
							std::make_unique<SvgRasterizationSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<SvgRasterizationMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
			}
		}

		if (_benchmarks.empty())
		{
			throw ElpidaException("Missing benchmarks");
		}

		_model.SetTotalBenchmarks(4);
	}

	void FullBenchmarkController::RunAsync()
	{
		if (_running.load(std::memory_order_acquire)) return;

		if (_runnerThread.joinable())
		{
			_runnerThread.join();
		}

		_running.store(true, std::memory_order_release);
		_cancelling.store(false, std::memory_order_release);

		_model.SetTotalBenchmarks(_benchmarks.size() * _runConfigurationModel.GetIterationsToRun());
		_runnerThread = std::thread([this]
		{
			_model.SetRunning(true);
			std::vector<FullBenchmarkResultModel> thisResults;
			thisResults.reserve(_runConfigurationModel.GetIterationsToRun());
			try
			{
				for (std::size_t i = 0; i < _runConfigurationModel.GetIterationsToRun(); ++i)
				{
					Score singleCoreScore = 0.0;
					Score multiCoreScore = 0.0;
					Score memoryScore = 0.0;
					std::vector<BenchmarkResultModel> benchmarkResults;

					for (auto& benchmark : _benchmarks)
					{
						_model.SetCurrentRunningBenchmark(benchmark->GetName());

						auto result = benchmark->Run();

						singleCoreScore += result.GetSingleCoreScore();
						multiCoreScore += result.GetMultiThreadScore();
						memoryScore += result.GetMemoryScore();
						benchmarkResults.push_back(std::move(result.GetBenchmarkResult()));
					}

					auto totalScore = singleCoreScore + multiCoreScore + multiCoreScore;

					auto result = FullBenchmarkResultModel(std::move(benchmarkResults), totalScore, singleCoreScore,
							multiCoreScore, memoryScore);
					_model.Add(result);
					thisResults.push_back(std::move(result));
				}

			}
			catch (const ElpidaException& ex)
			{
				if (!_cancelling.load(std::memory_order_acquire))
				{
					_messageService.ShowError("Failed to run benchmark: " + std::string(ex.what()));
				}
			}

			if (_runConfigurationModel.IsGenerateHtmlReport() && !thisResults.empty())
			{
				std::string fileName = "Full Benchmark ";
				fileName
						.append(" ")
						.append(std::to_string(time(nullptr)))
						.append(".html");

				auto path = _pathsService.GetDownloadStoragePath() / "Elpida Exported Reports" / fileName;

				_resultsHTMLReporter.WriteFullBenchmarkReport(thisResults, path);

				_desktopService.OpenFile(path);
			}

			_model.SetRunning(false);
			_running.store(false, std::memory_order_release);
		});
	}

	void FullBenchmarkController::StopRunning()
	{
		_cancelling.store(true, std::memory_order_release);
		_benchmarkExecutionService.StopCurrentExecution();
	}

	FullBenchmarkController::~FullBenchmarkController()
	{
		if (_runnerThread.joinable())
		{
			StopRunning();
			_runnerThread.join();
		}
	}

	void FullBenchmarkController::ClearResults()
	{
		_model.Clear();
	}

	void FullBenchmarkController::SaveResults(const std::filesystem::path& filePath)
	{
		std::fstream file(filePath.c_str(), std::ios::trunc | std::fstream::out);
		file << _resultSerializer.Serialize(_model);
	}
} // Elpida
// Application