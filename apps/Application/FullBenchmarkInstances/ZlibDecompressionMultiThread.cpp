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

#include "ZlibDecompressionMultiThread.hpp"

#include "Models/BenchmarkRunConfigurationModel.hpp"

#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Core/BenchmarkExecutionService.hpp"
#include "Elpida/Platform/OsUtilities.hpp"

namespace Elpida::Application
{
	const Score BaseScore = 32.61 * std::mega::num;

	std::string ZlibDecompressionMultiThread::GetName() const
	{
		return "Zlib Decompression (Multi Thread)";
	}

	std::string ZlibDecompressionMultiThread::GetUuid() const
	{
		return "39d94225-34aa-479b-b9cb-b6e104fb17b9";
	}

	FullBenchmarkInstanceResult ZlibDecompressionMultiThread::Run()
	{
		_benchmark.GetConfigurations()[0].SetValue(OsUtilities::GetExecutableDirectory() / "assets/lorem-ipsum.z");
		_benchmark.GetConfigurations()[1].SetValue("./lorem-ipsum.txt");

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

	ZlibDecompressionMultiThread::ZlibDecompressionMultiThread(const BenchmarkModel& benchmark,
			const TimingModel& timingModel,
			const TopologyModel& topologyModel, const MemoryInfoModel& memoryInfoModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, memoryInfoModel, executionService)
	{

	}
} // Elpida::Application