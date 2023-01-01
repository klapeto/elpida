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
#include <fstream>

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

#define NON_EXIT_CODE (-1)

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

json getTaskSpecificationJson(const TaskSpecification& taskSpec);

void printVersion()
{
	std::cout << "Elpida Benchmark Metadata Dumper: " << ELPIDA_VERSION << std::endl;
	std::cout << "Compiler: " << ELPIDA_COMPILER_NAME << " Version: " << ELPIDA_COMPILER_VERSION << std::endl;
}

void printHelp()
{
	std::cout << "Elpida Benchmark Metadata Dumper: " << ELPIDA_VERSION << std::endl;
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
				std::cerr << "--directory requires a path eg: --directory=./Benchmarks" << std::endl;
				return EXIT_FAILURE;
			}
			break;
		case '?':
			return EXIT_FAILURE;
		default:
			break;
		}
	}

    if (configurationModel.getBenchmarksPath().empty())
    {
        std::cerr << "--directory requires a path eg: --directory=./Benchmarks" << std::endl;
        return EXIT_FAILURE;
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

static json getResultSpec(const ResultSpecification& resultSpecification)
{
	json resultJ;

	resultJ["name"] = resultSpecification.getName();
	resultJ["description"] = resultSpecification.getDescription();
	resultJ["type"] = resultSpecification.getType();
	resultJ["aggregation"] = resultSpecification.getAggregationType();
	resultJ["unit"] = resultSpecification.getUnit();

	return resultJ;
}

static json getBenchmarkScoreSpecification(const BenchmarkScoreSpecification& scoreSpecification)
{
	json returnJson;

	returnJson["unit"] = scoreSpecification.getUnit();
	returnJson["comparison"] = scoreSpecification.getComparison();

	return returnJson;
}

json getTaskSpecificationJson(const TaskSpecification& taskSpec)
{
	json taskSpecJ;

	taskSpecJ["uuid"] = taskSpec.getUuid();
	taskSpecJ["name"] = taskSpec.getName();
	taskSpecJ["description"] = taskSpec.getDescription();

	taskSpecJ["result"] = getResultSpec(taskSpec.getResultSpecification());

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
	return taskSpecJ;
}

static void DumpJsonToFile(const std::string& filename, const json& json)
{
	std::fstream file;
	file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	file.open(filename, std::ios::out | std::ios::trunc);
	file << json.dump();
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

    std::unordered_map<std::string, Reference<const TaskSpecification>> taskSpecifications;

    json root = json::object();
	json benchmarksGroupsJ = json::array();
	for (const auto& benchmarkGroup : benchmarkGroups)
	{
		json benchmarkGroupJ;
		benchmarkGroupJ["name"] = benchmarkGroup.getValue().getName();
		benchmarkGroupJ["library"] = benchmarkGroup.getValue().getLibraryPath();
        json benchmarksArrayJ = json::array();
		for (auto& benchmark: benchmarkGroup.getValue().getBenchmarks())
		{
			json benchmarkJ;
			benchmarkJ["uuid"] = benchmark->getUuid();
			benchmarkJ["name"] = benchmark->getName();

			benchmarkJ["scoreSpecification"] = getBenchmarkScoreSpecification(benchmark->getScoreSpecification());

			{
				json benchmarkTasksJ = json::array();
				for (auto& task: benchmark->getTaskBuilders())
				{
					auto& taskSpec = task.getTaskSpecification();

                    taskSpecifications.try_emplace(taskSpec.getUuid(),taskSpec);

					json taskBuilderJ;
					taskBuilderJ["uuid"] = taskSpec.getUuid();
					taskBuilderJ["canBeMultiThreaded"] = task.canBeMultiThreaded();
					taskBuilderJ["canBeDisabled"] = task.canBeDisabled();
					taskBuilderJ["iterationsToRun"] = task.getIterationsToRun();
					taskBuilderJ["isCountedOnResults"] = task.shouldBeCountedOnResults();

					benchmarkTasksJ.push_back(std::move(taskBuilderJ));
				}
				benchmarkJ["tasks"] = benchmarkTasksJ;
			}

            benchmarksArrayJ.push_back(std::move(benchmarkJ));
		}
        benchmarkGroupJ["benchmarks"] = std::move(benchmarksArrayJ);
        benchmarksGroupsJ.push_back(std::move(benchmarkGroupJ));
	}

    root["benchmarkGroups"] = std::move(benchmarksGroupsJ);

    json tasksJ = json::array();

    for (auto& taskSpec: taskSpecifications)
    {
        tasksJ.push_back(getTaskSpecificationJson(taskSpec.second.get()));
    }
    root["tasks"] = std::move(tasksJ);

	DumpJsonToFile("benchmarks.json", root);

	//std::cout << root.dump() << std::endl;

	return EXIT_SUCCESS;
}


