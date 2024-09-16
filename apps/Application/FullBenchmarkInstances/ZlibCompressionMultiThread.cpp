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

#include "ZlibCompressionMultiThread.hpp"

#include "Models/BenchmarkRunConfigurationModel.hpp"

#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Core/BenchmarkExecutionService.hpp"
#include "Elpida/Platform/OsUtilities.hpp"

namespace Elpida::Application
{
	const Score BaseScore = 11.47 * std::mega::num;

	std::string ZlibCompressionMultiThread::GetName() const
	{
		return "Zlib Compression (Multi Thread)";
	}

	std::string ZlibCompressionMultiThread::GetUuid() const
	{
		return "1b4532fd-14f0-41af-ab33-5e4e3803f79b";
	}

	FullBenchmarkInstanceResult ZlibCompressionMultiThread::Run()
	{
		_benchmark.GetConfigurations()[0].SetValue(OsUtilities::GetExecutableDirectory() / "assets/lorem-ipsum.txt");
		_benchmark.GetConfigurations()[1].SetValue("./lorem-ipsum.z");

		auto svgRasterizationSingle = _executionService.Execute(
				_benchmark,
				{},
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				false,
				false,
				ConcurrencyMode::ShareInput);

		auto rasterizationResult = svgRasterizationSingle.GetResult() / BaseScore;
		Score multiThreadScore = rasterizationResult;

		return FullBenchmarkInstanceResult(std::move(svgRasterizationSingle), 0, multiThreadScore);
	}

	ZlibCompressionMultiThread::ZlibCompressionMultiThread(const BenchmarkModel& benchmark, const TimingModel& timingModel,
			const TopologyModel& topologyModel, const MemoryInfoModel& memoryInfoModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, memoryInfoModel, executionService)
	{

	}
} // Elpida::Application