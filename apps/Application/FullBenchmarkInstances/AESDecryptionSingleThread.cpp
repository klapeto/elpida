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

#include "AESDecryptionSingleThread.hpp"

#include "Models/BenchmarkRunConfigurationModel.hpp"

#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Core/BenchmarkExecutionService.hpp"
#include "Elpida/Platform/OsUtilities.hpp"

namespace Elpida::Application
{
	const Score BaseScore = 190.68 * std::mega::num;

	std::string AESDecryptionSingleThread::GetName() const
	{
		return "AES Decryption (Single Thread)";
	}

	std::string AESDecryptionSingleThread::GetUuid() const
	{
		return "9a44b227-39b1-41d6-b8fc-18243d9c9744";
	}

	FullBenchmarkInstanceResult AESDecryptionSingleThread::Run()
	{
		_benchmark.GetConfigurations()[0].SetValue((OsUtilities::GetExecutableDirectory() / "assets" / "lorem-ipsum.enc").string());
		_benchmark.GetConfigurations()[1].SetValue("ab909b43d4cc43388ed2c98d261b082");
		_benchmark.GetConfigurations()[2].SetValue("./lorem-ipsum.dec");
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

	AESDecryptionSingleThread::AESDecryptionSingleThread(const BenchmarkModel& benchmark,
			const TimingModel& timingModel,
			const TopologyModel& topologyModel,
			const MemoryInfoModel& memoryInfoModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, memoryInfoModel, executionService)
	{

	}
} // Elpida::Application