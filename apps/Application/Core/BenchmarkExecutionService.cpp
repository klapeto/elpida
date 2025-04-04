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
// Created by klapeto on 17/4/2023.
//

#include "BenchmarkExecutionService.hpp"

#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Platform/AsyncPipeReader.hpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "Models/Benchmark/BenchmarkModel.hpp"

#include "json.hpp"

#include <locale>
#include <sstream>
#include <vector>
#include <fstream>

namespace Elpida::Application
{
	BenchmarkResultModel
	BenchmarkExecutionService::Execute(const BenchmarkModel& benchmarkModel,
			const std::vector<std::size_t>& affinity,
			double nowOverheadSeconds,
			double loopOverheadSeconds,
			bool numaAware,
			bool pinThreads,
			ConcurrencyMode concurrencyMode,
			double minimumMicroTaskDuration)
	{
		std::vector<std::string> configuration;
		std::unordered_map<std::string, std::string> configurationMap;
		configuration.reserve(benchmarkModel.GetConfigurations().size());

		for (auto& config : benchmarkModel.GetConfigurations())
		{
			configuration.emplace_back(config.GetValue());
			configurationMap.insert(std::make_pair(config.GetName(), config.GetValue()));
		}

		auto toString = [](double d)
		{
			std::ostringstream doubleToStringAccumulator;
			doubleToStringAccumulator.imbue(std::locale());
			doubleToStringAccumulator << d;
			return doubleToStringAccumulator.str();
		};

		std::vector<std::string> arguments
				{
						"--index=" + std::to_string(benchmarkModel.GetBenchmarkIndex()),
						"--now-overhead=" + toString(nowOverheadSeconds),
						"--loop-overhead=" + toString(loopOverheadSeconds),
						"--microtask-duration=" + toString(minimumMicroTaskDuration),
						"--format=json"
				};

		if (!affinity.empty())
		{
			std::ostringstream affinityAccumulator;
			affinityAccumulator << "--affinity=";
			for (auto processor : affinity)
			{
				affinityAccumulator << processor << ',';
			}
			auto affinityStr = affinityAccumulator.str();

			arguments.push_back(affinityStr.substr(0, affinityStr.size() - 1));
		}

		for (auto& value : configuration)
		{
			arguments.push_back(std::string("--config=\"").append(value).append("\""));
		}

		if (numaAware)
		{
			arguments.emplace_back("--numa-aware");
		}

		if (pinThreads)
		{
			arguments.emplace_back("--pin-threads");
		}

		arguments.emplace_back(
				std::string("--concurrency-mode=").append(std::to_string(static_cast<std::size_t>(concurrencyMode))));

		_currentProcess = Process(benchmarkModel.GetFilePath(), arguments, true, true);

		AsyncPipeReader stdOutReader(_currentProcess.GetStdOut());
		AsyncPipeReader stdErrReader(_currentProcess.GetStdErr());
		stdOutReader.StartReading();
		stdErrReader.StartReading();
		try
		{
			_currentProcess.WaitToExit();
		}
		catch (const ElpidaException& ex)
		{
			stdErrReader.StopReading();
			throw ElpidaException(stdErrReader.GetString());
		}

		stdOutReader.StopReading();
		stdErrReader.StopReading();

		auto serializedResult = stdOutReader.GetString();

		auto json = nlohmann::json::parse(serializedResult);

		auto isThroughput = benchmarkModel.GetResultType() == ResultType::Throughput;
		auto result = json["result"].template get<double>();

		std::string resultUnit = isThroughput ? benchmarkModel.GetResultUnit() + "/s" : benchmarkModel.GetResultUnit();

		return BenchmarkResultModel(benchmarkModel,result,std::move(configurationMap));
	}

	void BenchmarkExecutionService::StopCurrentExecution()
	{
		try
		{
			_currentProcess.Terminate();
		}
		catch (...)
		{
			// if process not exists we do not care
		}
	}
} // Application