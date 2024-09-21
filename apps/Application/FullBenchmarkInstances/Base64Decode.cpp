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

#include "Base64Decode.hpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "Models/Benchmark/BenchmarkModel.hpp"

namespace Elpida::Application
{
	Base64Decode::Base64Decode(const std::string& uuid,
			Score baseScore,
			bool multiThreaded,
			const BenchmarkModel& benchmark,
			const TimingModel& timingModel,
			const TopologyModel& topologyModel,
			const MemoryInfoModel& memoryInfoModel,
			const BenchmarkRunConfigurationModel& runConfigurationModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance("Base64 Decode", uuid, baseScore, multiThreaded, benchmark, timingModel,
			topologyModel,
			memoryInfoModel,
			runConfigurationModel,
			executionService)
	{
	}

	void Base64Decode::Configure()
	{
		_benchmark.GetConfigurations()[0].SetValue("2048");
	}

	ConcurrencyMode Base64Decode::GetMultiThreadConcurrencyMode() const
	{
		return ConcurrencyMode::CopyInput;
	}
} // Elpida::Application