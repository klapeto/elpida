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
// Created by klapeto on 11/5/2024.
//

#include "FullBenchmarkInstance.hpp"

#include "Core/BenchmarkExecutionService.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"

namespace Elpida::Application
{

	FullBenchmarkInstanceResult::FullBenchmarkInstanceResult(BenchmarkResultModel&& benchmarkResult,
			Score score)
			:_benchmarkResult(std::move(benchmarkResult)), _score(score)
	{
	}

	BenchmarkResultModel& FullBenchmarkInstanceResult::GetBenchmarkResult()
	{
		return _benchmarkResult;
	}

	Score FullBenchmarkInstanceResult::GetScore() const
	{
		return _score;
	}

	FullBenchmarkInstance::FullBenchmarkInstance(
			std::string name,
			std::string uuid,
			Score baseScore,
			bool multiThreaded,
			const BenchmarkModel& benchmark,
			const TimingModel& timingModel,
			const TopologyModel& topologyModel,
			const MemoryInfoModel& memoryInfoModel,
			const BenchmarkRunConfigurationModel& runConfigurationModel,
			BenchmarkExecutionService& executionService
			)
			:_benchmark(benchmark),
			 _timingModel(timingModel),
			 _topologyModel(topologyModel),
			 _memoryInfoModel(memoryInfoModel),
			 _runConfigurationModel(runConfigurationModel),
			 _executionService(executionService),
			 _uuid(std::move(uuid)),
			 _baseScore(baseScore),
			 _multiThread(multiThreaded)
	{
		_name = std::move(name);
		_name += (multiThreaded ? " (Multi thread)" : " (Single thread)");
	}

	FullBenchmarkInstanceResult FullBenchmarkInstance::Run()
	{
		Configure();
		if (_multiThread)
		{
			return RunMultiThread();
		}
		else
		{
			return RunSingleThread();
		}
	}

	FullBenchmarkInstanceResult FullBenchmarkInstance::RunSingleThread() const
	{
		auto benchmarkResult = _executionService.Execute(
				_benchmark,
				{ _topologyModel.GetFastestProcessor() },
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				false,
				false,
				ConcurrencyMode::None,
				Seconds(5).count()
		);

		auto thisResult = benchmarkResult.GetResult() / _baseScore;

		return FullBenchmarkInstanceResult(std::move(benchmarkResult), thisResult);
	}

	FullBenchmarkInstanceResult FullBenchmarkInstance::RunMultiThread() const
	{
		auto benchmarkResult = _executionService.Execute(
				_benchmark,
				{},
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				false,
				false,
				GetMultiThreadConcurrencyMode(),
				Seconds(5).count()
				);

		auto thisResult = benchmarkResult.GetResult() / _baseScore;

		return FullBenchmarkInstanceResult(std::move(benchmarkResult),thisResult);

	}

	const std::string& FullBenchmarkInstance::GetName() const
	{
		return _name;
	}

	const std::string& FullBenchmarkInstance::GetUuid() const
	{
		return _uuid;
	}

	bool FullBenchmarkInstance::IsMultiThread() const
	{
		return _multiThread;
	}
} // Application
// Elpida