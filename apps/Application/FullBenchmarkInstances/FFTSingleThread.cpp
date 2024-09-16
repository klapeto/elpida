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

#include "FFTSingleThread.hpp"

#include "Models/BenchmarkRunConfigurationModel.hpp"

#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Core/BenchmarkExecutionService.hpp"

namespace Elpida::Application
{
	const Score BaseScore = 9.76 * std::mega::num;

	std::string FFTSingleThread::GetName() const
	{
		return "FFT (Single Thread)";
	}

	std::string FFTSingleThread::GetUuid() const
	{
		return "a8946ba5-049f-4626-9b1a-d5d31211cea1";
	}

	FullBenchmarkInstanceResult FFTSingleThread::Run()
	{
		_benchmark.GetConfigurations()[0].SetValue("2048");

		auto benchmarkResult = _executionService.Execute(
				_benchmark,
				{ _topologyModel.GetFastestProcessor() },
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				false,
				false,
				ConcurrencyMode::None);

		auto thisResult = benchmarkResult.GetResult() / BaseScore;
		Score singleCoreScore = thisResult;

		return FullBenchmarkInstanceResult(std::move(benchmarkResult), singleCoreScore, 0);
	}

	FFTSingleThread::FFTSingleThread(const BenchmarkModel& benchmark, const TimingModel& timingModel,
			const TopologyModel& topologyModel, const MemoryInfoModel& memoryInfoModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, memoryInfoModel, executionService)
	{

	}
} // Elpida::Application