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

namespace Elpida::Application
{
	static inline const char* ExecutablePath = "./elpida-executor"
											   #if defined(ELPIDA_WINDOWS)
											   ".exe";
											   #elif defined(ELPIDA_UNIX)
											   "";
#endif

	BenchmarkResultModel
	BenchmarkExecutionService::Execute(const BenchmarkModel& benchmarkModel,
		const std::vector<std::size_t>& affinity,
		double nowOverheadNanoseconds,
		double loopOverheadNanoseconds,
		double virtualCallOverheadNanoseconds)
	{

		std::vector<std::string> configuration;
		configuration.reserve(benchmarkModel.GetConfigurations().size());

		for (auto& config : benchmarkModel.GetConfigurations())
		{
			configuration.emplace_back(config.GetValue());
		}

		std::locale::global(std::locale());
		std::vector<std::string> arguments
			{
				std::string("--module=") + "\"" + benchmarkModel.GetFilePath() + "\"",
				"--index=" + std::to_string(benchmarkModel.GetIndex()),
				"--now-nanoseconds=" + std::to_string(nowOverheadNanoseconds),
				"--loop-nanoseconds=" + std::to_string(loopOverheadNanoseconds),
				"--virtual-nanoseconds=" + std::to_string(virtualCallOverheadNanoseconds),
				"--format=json"
			};

		std::ostringstream affinityAccumulator;
		affinityAccumulator << "--affinity=";
		for (auto processor : affinity)
		{
			affinityAccumulator << processor << ',';
		}
		auto affinityStr = affinityAccumulator.str();

		arguments.push_back(affinityStr.substr(0, affinityStr.size() - 1));

		for (auto& value : configuration)
		{
			arguments.push_back(std::string("--config=\"").append(value).append("\""));
		}

		std::locale::global(std::locale(""));
		_currentProcess = Process(ExecutablePath, arguments, true, true);

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

		nlohmann::json json = nlohmann::json::parse(serializedResult);

		auto score = json["score"].template get<double>();
		auto taskResultsJ = json["taskResults"];

		std::vector<TaskResultModel> taskResults;
		taskResults.reserve(taskResultsJ.size());

		for (auto& taskJ : taskResultsJ)
		{
			taskResults.emplace_back(
					NanoSeconds(taskJ["durationNanoseconds"].template get<double>()),
					taskJ["dataSize"].get<std::size_t>()
			);
		}
		return BenchmarkResultModel(benchmarkModel, score, std::move(taskResults));
	}

	void BenchmarkExecutionService::StopCurrentExecution()
	{
		_currentProcess.Terminate();
	}
} // Application