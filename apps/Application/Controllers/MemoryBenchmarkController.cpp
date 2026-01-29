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
// Created by klapeto on 18/6/2024.
//

#include "MemoryBenchmarkController.hpp"

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
#include <fstream>

namespace Elpida::Application
{
	MemoryBenchmarkController::MemoryBenchmarkController(MemoryBenchmarkModel& model,
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
			_memoryLatencyBenchmark(nullptr),
			_memoryReadBandwidthBenchmark(nullptr)
	{

		for (auto& group : benchmarkGroups)
		{
			for (auto& benchmark : group.GetBenchmarks())
			{
				auto& name = benchmark.GetName();
				if (name == "Memory latency")
				{
					_memoryLatencyBenchmark = &benchmark;
				}
				else if (name == "Memory read bandwidth")
				{
					_memoryReadBandwidthBenchmark = &benchmark;
				}
			}
		}
		if (_memoryLatencyBenchmark == nullptr || _memoryReadBandwidthBenchmark == nullptr)
		{
			throw ElpidaException("Missing Memory overhead benchmarks");
		}

		auto str = _settingsService.Get("MemoryBenchmark_Multiplier");
		if (!str.empty())
		{
			try
			{
				_model.SetMultiplier(ValueUtilities::StringToDoubleInvariant(str));
			}
			catch (...)
			{
				// ignored
			}
		}

		str = _settingsService.Get("MemoryBenchmark_InitialBytes");
		if (!str.empty())
		{
			try
			{
				_model.SetInitialBytes(std::stol(str));
			}
			catch (...)
			{
				// ignored
			}
		}


		str = _settingsService.Get("MemoryBenchmark_Iterations");
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

		str = _settingsService.Get("MemoryBenchmark_Type");
		if (!str.empty())
		{
			try
			{
				_model.SetBenchmarkType(static_cast<MemoryBenchmarkType>(std::stol(str)));
			}
			catch (...)
			{
				// ignored
			}
		}
	}

	void MemoryBenchmarkController::StopRunning()
	{
		_cancelling.store(true, std::memory_order_release);
		_benchmarkExecutionService.StopCurrentExecution();
	}

	void MemoryBenchmarkController::RunAsync()
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
			std::vector<MemoryBenchmarkResultModel> thisResults;
			thisResults.reserve(_targetSizes.size());
			try
			{
				CalculateTargetSizesAndProcessors();
				_model.SetTotalBenchmarks(_targetSizes.size());
				auto benchmark = GetBenchmarkToRun();
				if (benchmark == nullptr)
				{
					_messageService.ShowError("Could not run benchmark. Benchmark is missing");
					return;
				}
				for (unsigned long size : _targetSizes)
				{
					auto name = benchmark->GetName();
					_model.SetCurrentRunningBenchmark(name.append(" ").append(ValueUtilities::ToIEC(size).append("B")));
					auto result = RunBenchmark(benchmark, size);

					_model.Add(MemoryBenchmarkResultModel(size, result.GetResult()));
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

	void MemoryBenchmarkController::GenerateHtmlReport(
			const std::vector<MemoryBenchmarkResultModel>& thisResults) const
	{

	}

	void MemoryBenchmarkController::CalculateTargetSizesAndProcessors()
	{
		_targetSizes.clear();
		std::size_t size = _model.GetInitialBytes();

		for (std::size_t i = 0; i < _model.GetIterations(); ++i)
		{
			_targetSizes.push_back(size);
			size = std::ceil(size * _model.GetMultiplier());
		}

		_targetProcessors.clear();
		for (auto& processor : _topologyModel.GetSelectedLeafNodes())
		{
			_targetProcessors.push_back(processor.get().GetOsIndex().value());
		}
	}

	MemoryBenchmarkController::~MemoryBenchmarkController()
	{
		if (_runnerThread.joinable())
		{
			StopRunning();
			_runnerThread.join();
		}
	}

	void MemoryBenchmarkController::SetIterations(size_t iterations)
	{
		_model.SetIterations(iterations);
		_settingsService.Set("MemoryBenchmark_Iterations", std::to_string(iterations));
	}

	void MemoryBenchmarkController::SetMultiplier(double multiplier)
	{
		_model.SetMultiplier(multiplier);
		_settingsService.Set("MemoryBenchmark_Multiplier", ValueUtilities::DoubleToStringInvariant(multiplier));
	}

	void MemoryBenchmarkController::SetInitialBytes(size_t initialBytes)
	{
		_model.SetInitialBytes(initialBytes);
		_settingsService.Set("MemoryBenchmark_InitialBytes", std::to_string(initialBytes));
	}

	BenchmarkResultModel MemoryBenchmarkController::RunBenchmark(const BenchmarkModel* benchmark,
			std::size_t size) const
	{
		auto sizeStr = std::to_string(size);
		benchmark->GetConfigurations()[0].SetValue(sizeStr);
		return _benchmarkExecutionService.Execute(
				*benchmark,
				_targetProcessors,
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				_runConfigurationModel.IsNumaAware(),
				_runConfigurationModel.IsPinThreads(),
				_runConfigurationModel.GetConcurrencyMode(),
				_runConfigurationModel.GetMinimumMicroTaskDuration().count());

	}

	const BenchmarkModel* MemoryBenchmarkController::GetBenchmarkToRun() const
	{
		switch (_model.GetBenchmarkType())
		{
		case MemoryBenchmarkType::MemoryLatency:
			return _memoryLatencyBenchmark;
		case MemoryBenchmarkType::MemoryReadBandwidth:
			return _memoryReadBandwidthBenchmark;
		}
		return nullptr;
	}

	void MemoryBenchmarkController::SetBenchmarkType(MemoryBenchmarkType type)
	{
		_model.SetBenchmarkType(type);
		_settingsService.Set("MemoryBenchmark_Type", std::to_string(static_cast<int>(type)));
	}

	void MemoryBenchmarkController::ClearResults()
	{
		_model.Clear();
	}

	void MemoryBenchmarkController::SaveResults(const std::filesystem::path& filePath)
	{
		std::fstream file(filePath.c_str(), std::ios::trunc | std::fstream::out);

		file << _resultSerializer.Serialize(_model);
	}
} // Application
// Elpida