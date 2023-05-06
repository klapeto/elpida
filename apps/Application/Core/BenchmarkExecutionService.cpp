//
// Created by klapeto on 17/4/2023.
//

#include "BenchmarkExecutionService.hpp"

#include "Elpida/Core/Config.hpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Platform/Process.hpp"

#include <sstream>

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
		for (auto processor: affinity)
		{
			affinityAccumulator << processor << ',';
		}
		auto affinityStr = affinityAccumulator.str();

		arguments.push_back(affinityStr.substr(0, affinityStr.size() - 1));

		for (auto& value : configurations)
		{
			arguments.push_back(std::string("--config\"").append(value).append("\""));
		}

		Process process(ExecutablePath, arguments);
		process.WaitToExit();
		std::string str;
		process.GetStdOutput() >> str;
		return str;
	}

	void BenchmarkExecutionService::StopCurrentExecution()
	{

	}
} // Application