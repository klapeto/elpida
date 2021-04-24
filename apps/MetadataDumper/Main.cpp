/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 9/3/21.
//

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <getopt.h>

#include <Controllers/BenchmarksController.hpp>
#include <Models/Benchmarks/BenchmarksModel.hpp>
#include <Models/GlobalConfigurationModel.hpp>
#include <Models/BenchmarkConfigurationsCollectionModel.hpp>
#include <json.hpp>

#include <Elpida/Config.hpp>
#include <Elpida/SystemInfo/CpuInfo.hpp>
#include <Elpida/SystemInfo/TimingInfo.hpp>
#include <Elpida/SystemInfo/OsInfo.hpp>
#include <Elpida/SystemInfo/MemoryInfo.hpp>
#include <Elpida/SystemInfo/SystemTopology.hpp>
#include <Elpida/Utilities/Logging/Logger.hpp>
#include <Elpida/ServiceProvider.hpp>
#include <Elpida/Utilities/Logging/LogAppender.hpp>


#define NON_EXIT_CODE -1

using namespace Elpida;
using namespace nlohmann;

class ConsoleAppender : public LogAppender
{
public:

	void append(LogType logType,
		const Logger::TimeStamp& timeStamp,
		const std::string& message,
		const std::exception* exception) override
	{
		std::cout << getTypeStr(logType)
				  << "["
				  << timestampToString(timeStamp, "%H:%M:%S")
				  << "] "
				  << message;

		if (exception)
		{
			std::cout << " -> Ex: "
					  << exception->what();
		}

		std::cout << std::endl;
	}
	ConsoleAppender() = default;
	~ConsoleAppender() override = default;

private:
	static const char* getTypeStr(LogType logType)
	{
		switch (logType)
		{
		case LogType::Info:
			return "[Info]";
		case LogType::Warning:
			return "[Warning]";
		case LogType::Error:
			return "[Error]";
		}
		return "[???]";
	}

	static std::string timestampToString(const Logger::TimeStamp& time, const std::string& format)
	{
		std::time_t tt = std::chrono::system_clock::to_time_t(time);
		std::tm tm = *std::gmtime(&tt);
		std::stringstream ss;
		ss << std::put_time(&tm, format.c_str());
		return ss.str();
	}
};

void printVersion()
{
	std::cout << "Elpida Benchmark Medatada Dumper: " << ELPIDA_VERSION << std::endl;
	std::cout << "Compiler: " << ELPIDA_COMPILER_NAME << " Version: " << ELPIDA_COMPILER_VERSION << std::endl;
}

void printHelp()
{
	std::cout << "Elpida Benchmark Medatada Dumper: " << ELPIDA_VERSION << std::endl;
	std::cout << "Usage: " << std::endl;
	std::cout << "       -v, --version" << std::endl;
	std::cout << "           Prints the version and exits" << std::endl;
	std::cout << "       -h, --help" << std::endl;
	std::cout << "           Prints this help and exits" << std::endl;
	std::cout << "       -d=DIRECTORY, --directory=DIRECTORY" << std::endl;
	std::cout << "           Overrides the directory to load the Benchmarks from" << std::endl;
}

int processArgumentsAndCheckIfWeMustExit(GlobalConfigurationModel& configurationModel, int argC, char* argV[])
{
	enum Opts
	{
		Arg_Version,
		Arg_Help,
		Arg_Directory,
	};
	struct option options[] = {
		{ "version", no_argument, nullptr, 'v' },
		{ "help", no_argument, nullptr, 'h' },
		{ "directory", required_argument, nullptr, 'd' },
		{ nullptr, 0, nullptr, 0 }
	};

	int option_index = 0;
	int c = 0;
	while ((c = getopt_long(argC, argV, "v:h:d:", options, &option_index)) != -1)
	{
		switch (c)
		{
		case 'v':
			printVersion();
			return EXIT_SUCCESS;
		case 'h':
			printHelp();
			return EXIT_SUCCESS;
		case 'd':
			if (optarg != nullptr)
			{
				configurationModel.setBenchmarksPath(optarg);
			}
			else
			{
				std::cerr << "--directory requires a path eg: --director=./Benchmarks";
				return EXIT_FAILURE;
			}
			break;
		case '?':
			return EXIT_FAILURE;
		default:
			break;
		}
	}
	return NON_EXIT_CODE;
}

void loadDefaultGlobalConfiguration(GlobalConfigurationModel& configurationModel)
{
	configurationModel.transactional<GlobalConfigurationModel>([](GlobalConfigurationModel& configurationModel)
	{
		configurationModel.setBenchmarksPath(TASK_BATCH_DEBUG_DIR);
	});
}

int main(int argC, char** argV)
{

	GlobalConfigurationModel configurationModel;

	loadDefaultGlobalConfiguration(configurationModel);

	auto exitCode = processArgumentsAndCheckIfWeMustExit(configurationModel, argC, argV);

	if (exitCode != NON_EXIT_CODE)
	{
		return exitCode;
	}

	const CpuInfo& cpuInfo = CpuInfo::get();

	SystemTopology topology;
	MemoryInfo memoryInfo;
	OsInfo osInfo;
	TimingInfo timingInfo;
	Logger logger;
	ConsoleAppender consoleAppender;

	logger.addAppender(consoleAppender);

	ServiceProvider serviceProvider(cpuInfo, memoryInfo, topology, osInfo, timingInfo, logger);

	BenchmarksModel benchmarksModel;
	BenchmarkConfigurationsCollectionModel benchmarkConfigurationsModel;

	BenchmarksController
		controller(benchmarksModel, benchmarkConfigurationsModel, configurationModel, serviceProvider, logger);

	controller.reload();

	auto& benchmarkGroups = benchmarksModel.getItems();

	json root;

	json benchmarksJ = json::array();
	for (const auto& benchmarkGroup: benchmarkGroups)
	{
		for (auto& benchmark: benchmarkGroup.getValue().getBenchmarks())
		{
			json benchJ;
			benchJ["uuid"] = benchmark->getUuid();
			benchJ["name"] = benchmark->getName();
			{
				json tasksJ = json::array();
				for (auto& task: benchmark->getTaskBuilders())
				{
					auto& taskSpec = task.getTaskSpecification();

					json taskSpecJ;

					taskSpecJ["uuid"] = taskSpec.getUuid();
					taskSpecJ["name"] = taskSpec.getName();
					taskSpecJ["description"] = taskSpec.getDescription();

					{
						auto& resultSpec = taskSpec.getResultSpecification();
						json resultJ;

						resultJ["name"] = resultSpec.getName();
						resultJ["description"] = resultSpec.getDescription();
						resultJ["type"] = resultSpec.getType();
						resultJ["aggregation"] = resultSpec.getAggregationType();
						resultJ["unit"] = resultSpec.getUnit();

						taskSpecJ["result"] = std::move(resultJ);
					}

					if (taskSpec.acceptsInput())
					{
						auto& inputSpec = taskSpec.getInputDataSpecification();
						json inputJ;

						inputJ["name"] = inputSpec.getName();
						inputJ["description"] = inputSpec.getDescription();
						inputJ["unit"] = inputSpec.getUnit();

						if (!inputSpec.getRequiredPropertiesNames().empty())
						{
							json propsJ = json::array();

							for (auto& property: inputSpec.getRequiredPropertiesNames())
							{
								propsJ.push_back(property);
							}

							inputJ["requiredProperties"] = propsJ;
						}

						taskSpecJ["input"] = std::move(inputJ);
					}

					if (taskSpec.producesOutput())
					{
						auto& outputSpec = taskSpec.getOutputDataSpecification();
						json outputJ;

						outputJ["name"] = outputSpec.getName();
						outputJ["description"] = outputSpec.getDescription();
						outputJ["unit"] = outputSpec.getUnit();

						taskSpecJ["output"] = std::move(outputJ);
					}


					tasksJ.push_back(std::move(taskSpecJ));
				}
				benchJ["taskSpecifications"] = tasksJ;
			}

			benchmarksJ.push_back(std::move(benchJ));
		}
	}

	root["benchmarks"] = std::move(benchmarksJ);


	std::cout << root.dump() << std::endl;

	return EXIT_SUCCESS;
}