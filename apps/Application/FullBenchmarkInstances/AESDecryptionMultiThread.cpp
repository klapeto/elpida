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

#include "AESDecryptionMultiThread.hpp"

#include "Models/BenchmarkRunConfigurationModel.hpp"

#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Core/BenchmarkExecutionService.hpp"
#include "Elpida/Platform/OsUtilities.hpp"

namespace Elpida::Application
{
	const Score BaseScore = 339.81 * std::mega::num;

	std::string AESDecryptionMultiThread::GetName() const
	{
		return "AES Decryption (Multi Thread)";
	}

	std::string AESDecryptionMultiThread::GetUuid() const
	{
		return "eb8aa408-5d7f-4d8e-8a37-e57edc011c63";
	}

	FullBenchmarkInstanceResult AESDecryptionMultiThread::Run()
	{
		_benchmark.GetConfigurations()[0].SetValue(OsUtilities::GetExecutableDirectory() / "assets/lorem-ipsum.enc");
		_benchmark.GetConfigurations()[1].SetValue("ab909b43d4cc43388ed2c98d261b082");
		_benchmark.GetConfigurations()[2].SetValue("./lorem-ipsum.dec");

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

	AESDecryptionMultiThread::AESDecryptionMultiThread(const BenchmarkModel& benchmark, const TimingModel& timingModel,
			const TopologyModel& topologyModel, const MemoryInfoModel& memoryInfoModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, memoryInfoModel, executionService)
	{

	}
} // Elpida::Application