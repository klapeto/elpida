//
// Created by klapeto on 17/4/2023.
//

#include "BenchmarkExecutionService.hpp"

#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Platform/AsyncPipeReader.hpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "Models/BenchmarkResultModel.hpp"

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

	std::string
	BenchmarkExecutionService::Execute(const std::string& libraryPath,
		std::size_t index,
		const std::vector<std::size_t>& affinity,
		const std::vector<std::string>& configurations,
		double nowOverheadNanoseconds,
		double loopOverheadNanoseconds,
		double virtualCallOverheadNanoseconds)
	{
		setlocale(LC_ALL, nullptr);
		std::vector<std::string> arguments
			{
				std::string("--module=") + "\"" + libraryPath + "\"",
				"--index=" + std::to_string(index),
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

		for (auto& value : configurations)
		{
			arguments.push_back(std::string("--config=\"").append(value).append("\""));
		}

		setlocale(LC_ALL, "");
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

		return stdOutReader.GetString();
	}

	void BenchmarkExecutionService::StopCurrentExecution()
	{
		_currentProcess.Terminate();
	}
} // Application