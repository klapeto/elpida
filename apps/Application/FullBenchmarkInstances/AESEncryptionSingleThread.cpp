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

#include "AESEncryptionSingleThread.hpp"

#include "Models/BenchmarkRunConfigurationModel.hpp"

#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Core/BenchmarkExecutionService.hpp"
#include "Elpida/Platform/OsUtilities.hpp"

namespace Elpida::Application
{
	const Score BaseScore = 151.2 * std::mega::num;

	std::string AESEncryptionSingleThread::GetName() const
	{
		return "AES Encryption (Single Thread)";
	}

	std::string AESEncryptionSingleThread::GetUuid() const
	{
		return "59248a7c-5d7f-4f1e-b01f-3c3cbe2e7437";
	}

	FullBenchmarkInstanceResult AESEncryptionSingleThread::Run()
	{
		_benchmark.GetConfigurations()[0].SetValue(OsUtilities::GetExecutableDirectory() / "assets" / "lorem-ipsum.txt");
		_benchmark.GetConfigurations()[1].SetValue("ab909b43d4cc43388ed2c98d261b082");
		_benchmark.GetConfigurations()[2].SetValue("./lorem-ipsum.enc");

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

	AESEncryptionSingleThread::AESEncryptionSingleThread(const BenchmarkModel& benchmark,
			const TimingModel& timingModel,
			const TopologyModel& topologyModel, const MemoryInfoModel& memoryInfoModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, memoryInfoModel, executionService)
	{

	}
} // Elpida::Application