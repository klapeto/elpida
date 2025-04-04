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
#include "ResultSerializer.hpp"
#include "DataUploader.hpp"

#include "FullBenchmarkInstancesLoader.hpp"

#include <sstream>
#include <fstream>
#include <locale>
#include <string>
#include <cmath>

namespace Elpida::Application
{
	static const double SingleCoreWeight = 1.1;
	static const double MultiCoreWeight = 1.0;

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
			const DataUploader& dataUploader,
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
			_dataUploader(dataUploader),
			_messageService(messageService),
			_running(false),
			_cancelling(false)
	{
		_benchmarks = FullBenchmarkInstancesLoader::Load(benchmarkGroups, timingModel, topologyModel, memoryInfoModel, runConfigurationModel, benchmarkExecutionService, _missingBenchmarks);

		if (_benchmarks.empty())
		{
			throw ElpidaException("Missing benchmarks");
		}

		_model.SetTotalBenchmarks(_benchmarks.size());
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
			auto start = Timer::now();
			_model.SetRunning(true);
			std::vector<FullBenchmarkResultModel> thisResults;
			thisResults.reserve(_runConfigurationModel.GetIterationsToRun());
			try
			{
				for (std::size_t i = 0; i < _runConfigurationModel.GetIterationsToRun(); ++i)
				{
					Score singleThreadScore = 0.0;
					Score multiThreadScore = 0.0;
					std::size_t singleThreadScoresCount = 0;
					std::size_t multiThreadScoresCount = 0;
					std::vector<BenchmarkResultModel> benchmarkResults;

					for (auto& benchmark : _benchmarks)
					{
						_model.SetCurrentRunningBenchmark(benchmark->GetName());

						auto result = benchmark->Run();
						result.GetBenchmarkResult().SetUuid(benchmark->GetUuid());
						result.GetBenchmarkResult().SetInstanceName(benchmark->GetName());

						if (benchmark->IsMultiThread())
						{
							multiThreadScore += 1.0 / (result.GetScore() / benchmark->GetBaseScore());
							multiThreadScoresCount++;
						}
						else
						{
							singleThreadScore += 1.0 / (result.GetScore() / benchmark->GetBaseScore());
							singleThreadScoresCount++;
						}

						benchmarkResults.push_back(std::move(result.GetBenchmarkResult()));
					}

					// harmonic mean since we are averaging performance ratios
					singleThreadScore = singleThreadScoresCount / singleThreadScore;
					multiThreadScore = multiThreadScoresCount / multiThreadScore;

					auto totalScore = CalculateTotalScore(singleThreadScore, multiThreadScore);

					auto result = FullBenchmarkResultModel(std::move(benchmarkResults), totalScore, singleThreadScore,
							multiThreadScore);
					_model.Add(result);
					thisResults.push_back(std::move(result));

					if (_cancelling.load(std::memory_order_acquire))
					{
						break;
					}

					if (_runConfigurationModel.GetDelaySecondsBetweenRuns() > 0
						&& _runConfigurationModel.GetIterationsToRun() > i + 1)
					{
						_model.SetCurrentRunningBenchmark("Waiting...");
						std::this_thread::sleep_for(Seconds(_runConfigurationModel.GetDelaySecondsBetweenRuns()));
					}

					if (_cancelling.load(std::memory_order_acquire))
					{
						break;
					}
				}

			}
			catch (const ElpidaException& ex)
			{
				if (!_cancelling.load(std::memory_order_acquire))
				{
					_messageService.ShowError("Failed to run benchmark: " + std::string(ex.what()));
				}
			}

			auto end = Timer::now();
			PostHandleResults(thisResults, end - start);

			_model.SetRunning(false);
			_running.store(false, std::memory_order_release);
		});
	}

	Score FullBenchmarkController::CalculateTotalScore(Score singleCoreScore, Score multiCoreScore)
	{
		return std::pow(singleCoreScore, SingleCoreWeight) + std::pow(multiCoreScore, MultiCoreWeight);
	}

	void FullBenchmarkController::PostHandleResults(const std::vector<FullBenchmarkResultModel>& thisResults, Duration duration) const
	{
		if (!thisResults.empty())
		{
			if (_runConfigurationModel.IsGenerateHtmlReport())
			{
				try
				{
					GenerateHtmlReport(thisResults, duration);
				}
				catch (const std::exception& ex)
				{
					_messageService.ShowError(std::string("Failed to crate report: ") + ex.what());
				}
			}
			if (_runConfigurationModel.IsUploadResults() && _missingBenchmarks.empty())
			{
				try
				{
					_model.SetCurrentRunningBenchmark("Uploading...");
					auto url = _dataUploader.UploadResult(thisResults);
					if (_runConfigurationModel.IsOpenResult())
					{
						_desktopService.OpenUri(url);
					}
				}
				catch (const std::exception& ex)
				{
					_messageService.ShowError(ex.what());
				}
			}
		}
	}

	void FullBenchmarkController::GenerateHtmlReport(const std::vector<FullBenchmarkResultModel>& thisResults, Duration duration) const
	{
		try
		{
			auto path = _pathsService.GetFullReportOutputPath();

			_resultsHTMLReporter.WriteFullBenchmarkReport(thisResults, duration, path);

			_desktopService.OpenFile(path);
		}
		catch (const std::exception& ex)
		{
			_messageService.ShowError("Failed to create report: " + std::string(ex.what()));
		}
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

		std::vector<FullBenchmarkResultModel> thisResults;
		thisResults.reserve(_model.Size());
		for (auto& result : _model.GetItems())
		{
			thisResults.push_back(result.GetValue());
		}
		file << _resultSerializer.Serialize(thisResults);
	}

	const std::vector<std::string>& FullBenchmarkController::GetMissingBenchmarks() const
	{
		return _missingBenchmarks;
	}

	void FullBenchmarkController::WaitToCompleteRun()
	{
		if (!_running.load(std::memory_order_acquire)) return;

		if (_runnerThread.joinable())
		{
			_runnerThread.join();
		}
	}

	const std::vector<std::unique_ptr<FullBenchmarkInstance>>& FullBenchmarkController::GetBenchmarks() const
	{
		return _benchmarks;
	}
} // Elpida
// Application