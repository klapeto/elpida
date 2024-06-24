//
// Created by klapeto on 18/6/2024.
//

#include "MemoryOverheadCalculationController.hpp"

#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"

#include "Core/BenchmarkExecutionService.hpp"
#include "Core/MessageService.hpp"
#include "Core/ResultsHTMLReporter.hpp"
#include "Core/PathsService.hpp"
#include "Core/DesktopService.hpp"
#include "Core/SettingsService.hpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "ResultSerializer.hpp"

#include <cmath>

namespace Elpida::Application
{

	static std::size_t CalculateWorkingSetSize(double scale)
	{
		return (std::pow(std::ceil(scale * 1024.0), 2.0) * 32) + 1024;
		//return ((std::pow(std::ceil(scale * 1024.0), 2.0) * 4.0) * 2) + 2840;
	}

	static Score CalculateExpectedScore(Score initialScore, std::size_t initialSubSamples, std::size_t subSamples,
			double initialScale, double scale)
	{
		auto initialSize = std::ceil(1024 * initialScale);
		auto initialLoad = std::pow(initialSize, 2.0) * initialSubSamples;

		auto thisSize = std::ceil(1024 * scale);
		auto thisLoad = std::pow(thisSize, 2.0) * subSamples;
		return initialScore / (thisLoad / initialLoad);
		//return initialScore / std::pow(scale / initialScale, 2);
	}

	static double CalculateOverhead(Score expectedScore, Score currentScore)
	{
		return ((currentScore / expectedScore) - 1.0) * 100.0;
	}

	MemoryOverheadCalculationController::MemoryOverheadCalculationController(MemoryOverheadCalculationModel& model,
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
			_settingsService(settingsService),
			_messageService(messageService),
			_running(false),
			_cancelling(false),
			_benchmark(nullptr)
	{

		for (auto& group : benchmarkGroups)
		{
			for (auto& benchmark : group.GetBenchmarks())
			{
				auto& name = benchmark.GetName();
				if (name == "Svg Rasterization")
				{
					_benchmark = &benchmark;
				}
			}
		}
		if (_benchmark == nullptr)
		{
			throw ElpidaException("Missing Memory overhead benchmarks");
		}

		auto str = _settingsService.Get("MemoryOverhead_SubSamplesMultiplier");
		if (!str.empty())
		{
			try
			{
				_model.SetSubSamplesMultiplier(ValueUtilities::StringToDoubleInvariant(str));
			}
			catch (...)
			{
				// ignored
			}
		}

		str = _settingsService.Get("MemoryOverhead_ScaleMultiplier");
		if (!str.empty())
		{
			try
			{
				_model.SetScaleMultiplier(ValueUtilities::StringToDoubleInvariant(str));
			}
			catch (...)
			{
				// ignored
			}
		}

		str = _settingsService.Get("MemoryOverhead_InitialSubSamples");
		if (!str.empty())
		{
			try
			{
				_model.SetInitialSubSamples(std::stol(str));
			}
			catch (...)
			{
				// ignored
			}
		}

		str = _settingsService.Get("MemoryOverhead_InitialScale");
		if (!str.empty())
		{
			try
			{
				_model.SetInitialScale(ValueUtilities::StringToDoubleInvariant(str));
			}
			catch (...)
			{
				// ignored
			}
		}

		str = _settingsService.Get("MemoryOverhead_Iterations");
		if (!str.empty())
		{
			try
			{
				_model.SetIterations(std::stol(str));
			}
			catch (...)
			{
				// ignored
			}
		}
	}

	void MemoryOverheadCalculationController::StopRunning()
	{
		_cancelling.store(true, std::memory_order_release);
		_benchmarkExecutionService.StopCurrentExecution();
	}

	void MemoryOverheadCalculationController::RunAsync()
	{
		if (_running.load(std::memory_order_acquire)) return;

		if (_runnerThread.joinable())
		{
			_runnerThread.join();
		}

		_model.Clear();
		_running.store(true, std::memory_order_release);
		_cancelling.store(false, std::memory_order_release);

		_runnerThread = std::thread([this]
		{
			_model.SetRunning(true);
			std::vector<MemoryOverheadResultModel> thisResults;
			thisResults.reserve(_targetScales.size());
			try
			{
				CalculateTargetScales();
				_model.SetTotalBenchmarks(_targetScales.size());
				Score initialScore = -1;
				for (std::size_t i = 0; i < _targetScales.size(); ++i)
				{
					auto scale = _targetScales[i];
					auto subSamples = _targetSubSamples[i];

					auto subSamplesStr = std::to_string(subSamples);
					auto scaleStr = ValueUtilities::ToFixed(scale, 5);
					_benchmark->GetConfigurations()[0].SetValue("1");
					_benchmark->GetConfigurations()[1].SetValue(scaleStr);
					_benchmark->GetConfigurations()[2].SetValue(subSamplesStr);

					auto workingSetSize = CalculateWorkingSetSize(scale);

					_model.SetCurrentRunningBenchmark(std::string("Sub Samples: ")
							.append(subSamplesStr).append(" Scale: ").append(scaleStr).append(" (").append(
							ValueUtilities::ToIEC(workingSetSize) + "B)"));
					auto result = _benchmarkExecutionService.Execute(
							*_benchmark,
							_targetProcessors,
							_timingModel.GetNowOverhead().count(),
							_timingModel.GetLoopOverhead().count(),
							_timingModel.GetVirtualCallOverhead().count(),
							_runConfigurationModel.IsNumaAware(),
							_runConfigurationModel.IsPinThreads(),
							_runConfigurationModel.GetConcurrencyMode());


					if (initialScore == -1)
					{
						_model.Add(
								MemoryOverheadResultModel(workingSetSize, subSamples, scale, 0, result.GetScore(), 0));
						initialScore = result.GetScore();
					}
					else
					{
						auto expectedScore = CalculateExpectedScore(initialScore, _targetSubSamples.at(0), subSamples,
								_targetScales.at(0), scale);
						_model.Add(
								MemoryOverheadResultModel(workingSetSize, subSamples, scale, expectedScore,
										result.GetScore(), CalculateOverhead(expectedScore, result.GetScore())));
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

			if (_runConfigurationModel.IsGenerateHtmlReport() && !thisResults.empty())
			{
				GenerateHtmlReport(thisResults);
			}

			_model.SetRunning(false);
			_running.store(false, std::memory_order_release);
		});
	}

	void MemoryOverheadCalculationController::GenerateHtmlReport(
			const std::vector<MemoryOverheadResultModel>& thisResults) const
	{

	}

	void MemoryOverheadCalculationController::CalculateTargetScales()
	{
		_targetScales.clear();
		_targetProcessors.clear();
		_targetSubSamples.clear();
		std::size_t targetSize = 4096;

		const std::vector<std::reference_wrapper<const TopologyNodeModel>>* nodes;
		if (_topologyModel.GetSelectedLeafNodes().empty())
		{
			nodes = &_topologyModel.GetLeafNodes();
		}
		else
		{
			nodes = &_topologyModel.GetSelectedLeafNodes();
		}
		for (auto& processor : *nodes)
		{
			_targetProcessors.push_back(processor.get().GetOsIndex().value());
			auto& cache = processor.get().GetLastCache();
			if (!cache.has_value()) continue;
			auto cacheSize = cache.value().get().GetSize();
			if (!cacheSize.has_value())continue;
			targetSize = std::max(targetSize, cacheSize.value());
		}

		//double targetMaxScale = (targetSize * 4.0) / 12585752;

		double thisScale = _model.GetInitialScale();//* (_timingModel.GetIterationsPerSecond() / std::giga::num);
		std::size_t thisSubSamples = _model.GetInitialSubSamples();

		for (std::size_t i = 0; i < _model.GetIterations(); ++i)
		{
			_targetScales.push_back(thisScale);
			_targetSubSamples.push_back(thisSubSamples);
			thisScale *= _model.GetScaleMultiplier();
			thisSubSamples = std::round(thisSubSamples * _model.GetSubSamplesMultiplier());
		}
	}

	MemoryOverheadCalculationController::~MemoryOverheadCalculationController()
	{
		if (_runnerThread.joinable())
		{
			StopRunning();
			_runnerThread.join();
		}
	}

	void MemoryOverheadCalculationController::SetSubSamplesMultiplier(double subSamplesMultiplier)
	{
		_model.SetSubSamplesMultiplier(subSamplesMultiplier);
		_settingsService.Set("MemoryOverhead_SubSamplesMultiplier", ValueUtilities::DoubleToStringInvariant(subSamplesMultiplier));
	}

	void MemoryOverheadCalculationController::SetInitialSubSamples(size_t initialSubSamples)
	{
		_model.SetInitialSubSamples(initialSubSamples);
		_settingsService.Set("MemoryOverhead_InitialSubSamples", std::to_string(initialSubSamples));
	}

	void MemoryOverheadCalculationController::SetScaleMultiplier(double scaleMultiplier)
	{
		_model.SetScaleMultiplier(scaleMultiplier);
		_settingsService.Set("MemoryOverhead_ScaleMultiplier", ValueUtilities::DoubleToStringInvariant(scaleMultiplier));
	}

	void MemoryOverheadCalculationController::SetInitialScale(double initialScale)
	{
		_model.SetInitialScale(initialScale);
		_settingsService.Set("MemoryOverhead_InitialScale", ValueUtilities::DoubleToStringInvariant(initialScale));
	}

	void MemoryOverheadCalculationController::SetIterations(size_t iterations)
	{
		_model.SetIterations(iterations);
		_settingsService.Set("MemoryOverhead_Iterations", std::to_string(iterations));
	}
} // Application
// Elpida