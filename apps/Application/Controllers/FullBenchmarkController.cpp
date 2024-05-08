//
// Created by klapeto on 22/10/2023.
//

#include "FullBenchmarkController.hpp"

#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"

#include "Core/BenchmarkExecutionService.hpp"
#include "Core/MessageService.hpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "ResultSerializer.hpp"

#include <sstream>
#include <fstream>
#include <locale>
#include <string>

namespace Elpida::Application
{
	const double Divider = 1000000.0;
	const std::size_t RasterizationSamplesBase = 8192;

	FullBenchmarkController::FullBenchmarkController(FullBenchmarkModel& model,
			const TimingModel& overheadsModel,
			const BenchmarkRunConfigurationModel& runConfigurationModel,
			BenchmarkExecutionService& benchmarkExecutionService,
			const ResultSerializer& resultSerializer,
			MessageService& messageService,
			const std::vector<BenchmarkGroupModel>& benchmarkGroups)
			:
			Controller(model),
			_overheadsModel(overheadsModel),
			_runConfigurationModel(runConfigurationModel),
			_benchmarkExecutionService(benchmarkExecutionService),
			_resultSerializer(resultSerializer),
			_messageService(messageService),
			_running(false),
			_cancelling(false),
			_memoryLatency(nullptr),
			_memoryReadBandwidth(nullptr),
			_svgRasterization(nullptr)
	{
		for (auto& group : benchmarkGroups)
		{
			for (auto& benchmark : group.GetBenchmarks())
			{
				auto name = benchmark.GetName();
				if (name == "Memory latency")
				{
					_memoryLatency = &benchmark;
				}
				else if (name == "Memory read bandwidth")
				{
					_memoryReadBandwidth = &benchmark;
				}
				else if (name == "Svg Rasterization")
				{
					_svgRasterization = &benchmark;
				}
			}
		}
		if (!_memoryReadBandwidth || !_memoryLatency)
		{
			throw ElpidaException("Missing benchmarks: Memory benchmarks");
		}

		if (!_svgRasterization)
		{
			throw ElpidaException("Missing benchmarks: Svg Rasterization");
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

		_model.SetTotalBenchmarks(4 * _runConfigurationModel.GetIterationsToRun());
		_runnerThread = std::thread([this]
		{
			_model.SetRunning(true);
			for (std::size_t i = 0; i < _runConfigurationModel.GetIterationsToRun(); ++i)
			{
				std::vector<std::size_t> affinity;
				FullBenchmarkResultModel::Score singleCoreScore = 0.0;
				FullBenchmarkResultModel::Score multiCoreScore = 0.0;
				FullBenchmarkResultModel::Score memoryScore = 0.0;
				std::vector<BenchmarkResultModel> benchmarkResults;

				_model.SetCurrentRunningBenchmark(_svgRasterization->GetName() + " (Single Thread)");

				auto thisPath = OsUtilities::GetExecutableDirectory();

				auto targetSamples = std::max((std::size_t)1, (std::size_t)(RasterizationSamplesBase * ((double)_overheadsModel.GetIterationsPerSecond() / std::giga::num)));
				_svgRasterization->GetConfigurations()[0].SetValue("1");
				_svgRasterization->GetConfigurations()[1].SetValue("0.06");
				_svgRasterization->GetConfigurations()[2].SetValue(std::to_string(targetSamples));
				_svgRasterization->GetConfigurations()[3].SetValue("false");
				_svgRasterization->GetConfigurations()[4].SetValue("false");
				_svgRasterization->GetConfigurations()[5].SetValue("false");

				try
				{
					auto svgRasterizationSingle = _benchmarkExecutionService.Execute(
							*_svgRasterization,
							affinity,
							_overheadsModel.GetNowOverhead().count(),
							_overheadsModel.GetLoopOverhead().count(),
							_overheadsModel.GetVirtualCallOverhead().count(),
							1.0,
							1.0,
							false,
							false);

					auto& taskResults = svgRasterizationSingle.GetTaskResults();

					auto& rasterizationResult = taskResults[0];
					singleCoreScore +=
							rasterizationResult.GetInputSize() / rasterizationResult.GetDuration().count() / Divider;

					benchmarkResults.push_back(std::move(svgRasterizationSingle));
				}
				catch (const ElpidaException& ex)
				{
					_model.SetRunning(false);
					if (!_cancelling.load(std::memory_order_acquire))
					{
						_messageService.ShowError("Failed to run benchmark: " + std::string(ex.what()));
					}
					return;
				}

				_model.SetCurrentRunningBenchmark(_svgRasterization->GetName() + " (Multi Thread)");

				targetSamples *= 2;
				_svgRasterization->GetConfigurations()[0].SetValue(std::to_string(std::thread::hardware_concurrency()));
				_svgRasterization->GetConfigurations()[1].SetValue("0.06");
				_svgRasterization->GetConfigurations()[2].SetValue(std::to_string(targetSamples));
				_svgRasterization->GetConfigurations()[3].SetValue("true");
				_svgRasterization->GetConfigurations()[4].SetValue("true");
				_svgRasterization->GetConfigurations()[5].SetValue("true");

				try
				{
					auto svgRasterizationMulti = _benchmarkExecutionService.Execute(
								*_svgRasterization,
								affinity,
								_overheadsModel.GetNowOverhead().count(),
								_overheadsModel.GetLoopOverhead().count(),
								_overheadsModel.GetVirtualCallOverhead().count(),
								32.0,
								32.0,
								false,
								false);

					auto& taskResults = svgRasterizationMulti.GetTaskResults();

					auto& rasterizationResult = taskResults[0];
					multiCoreScore +=
							rasterizationResult.GetInputSize() / rasterizationResult.GetDuration().count() / Divider;

					benchmarkResults.push_back(std::move(svgRasterizationMulti));
				}
				catch (const ElpidaException& ex)
				{
					_model.SetRunning(false);
					if (!_cancelling.load(std::memory_order_acquire))
					{
						_messageService.ShowError("Failed to run benchmark: " + std::string(ex.what()));
					}
					return;
				}

				_model.SetCurrentRunningBenchmark(_memoryLatency->GetName());

				try
				{
					auto latencyResult = _benchmarkExecutionService.Execute(
								*_memoryLatency,
								affinity,
								_overheadsModel.GetNowOverhead().count(),
								_overheadsModel.GetLoopOverhead().count(),
								_overheadsModel.GetVirtualCallOverhead().count(),
								1.0,
								1.0,
								false,
								true);

					auto& taskResults = latencyResult.GetTaskResults();

					memoryScore += Divider / (latencyResult.GetScore() * std::nano::den);

					benchmarkResults.push_back(std::move(latencyResult));
				}
				catch (const ElpidaException& ex)
				{
					_model.SetRunning(false);
					if (!_cancelling.load(std::memory_order_acquire))
					{
						_messageService.ShowError("Failed to run benchmark: " + std::string(ex.what()));
					}
					return;
				}


				_model.SetCurrentRunningBenchmark(_memoryReadBandwidth->GetName());

				try
				{
					auto bandwidthResult =  _benchmarkExecutionService.Execute(
								*_memoryReadBandwidth,
								affinity,
								_overheadsModel.GetNowOverhead().count(),
								_overheadsModel.GetLoopOverhead().count(),
								_overheadsModel.GetVirtualCallOverhead().count(),
								1.0,
								1.0,
								false,
								true);


					memoryScore += bandwidthResult.GetScore() / Divider / 1000;

					benchmarkResults.push_back(std::move(bandwidthResult));
				}
				catch (const ElpidaException& ex)
				{
					_model.SetRunning(false);
					if (!_cancelling.load(std::memory_order_acquire))
					{
						_messageService.ShowError("Failed to run benchmark: " + std::string(ex.what()));
					}
					return;
				}

				FullBenchmarkResultModel resultModel(std::move(benchmarkResults),
						singleCoreScore + multiCoreScore + memoryScore, singleCoreScore, multiCoreScore, memoryScore);

				_model.Add(std::move(resultModel));
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