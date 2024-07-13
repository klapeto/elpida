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
			double virtualCallSeconds,
			bool numaAware,
			bool pinThreads,
			ConcurrencyMode concurrencyMode)
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
						std::string("--module=") + "\"" + benchmarkModel.GetFilePath() + "\"",
						"--index=" + std::to_string(benchmarkModel.GetBenchmarkIndex()),
						"--now-overhead=" + toString(nowOverheadSeconds),
						"--loop-overhead=" + toString(loopOverheadSeconds),
						"--virtual-overhead=" + toString(virtualCallSeconds),
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

		_currentProcess = Process(OsUtilities::GetExecutableDirectory() / "elpida-executor", arguments, true, true);

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