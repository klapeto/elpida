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

#include "MatrixInverseMultiThread.hpp"

#include "Models/BenchmarkRunConfigurationModel.hpp"

#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Core/BenchmarkExecutionService.hpp"

namespace Elpida::Application
{
	const Score BaseScore = 102.89 * std::mega::num;

	std::string MatrixInverseMultiThread::GetName() const
	{
		return "Matrix Inverse (Multi Thread)";
	}

	std::string MatrixInverseMultiThread::GetUuid() const
	{
		return "3faca5e9-d9ac-47a5-a8d4-c331666c6e6a";
	}

	FullBenchmarkInstanceResult MatrixInverseMultiThread::Run()
	{
		auto benchmarkResult = _executionService.Execute(
				_benchmark,
				{},
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				false,
				false,
				ConcurrencyMode::ShareInput);

		auto thisResult = benchmarkResult.GetResult() / BaseScore;
		Score multiThreadScore = thisResult;

		return FullBenchmarkInstanceResult(std::move(benchmarkResult), 0, multiThreadScore);
	}

	MatrixInverseMultiThread::MatrixInverseMultiThread(const BenchmarkModel& benchmark, const TimingModel& timingModel,
			const TopologyModel& topologyModel, const MemoryInfoModel& memoryInfoModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, memoryInfoModel, executionService)
	{

	}
} // Elpida::Application