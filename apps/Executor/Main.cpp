/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2022  Ioannis Panagiotopoulos
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
// Created by klapeto on 10/12/2022.
//

#include <cstdlib>
#include <iostream>
#include <getopt.h>

#include <Elpida/Config.hpp>
#include <Elpida/SystemInfo/CpuInfo.hpp>
#include <Elpida/SystemInfo/TimingInfo.hpp>
#include <Elpida/SystemInfo/OsInfo.hpp>
#include <Elpida/SystemInfo/MemoryInfo.hpp>
#include <Elpida/SystemInfo/SystemTopology.hpp>
#include <Elpida/Utilities/Logging/Logger.hpp>
#include <Elpida/ServiceProvider.hpp>
#include <Elpida/Utilities/Logging/LogAppender.hpp>
#include <Elpida/OffThreadExecutor.hpp>
#include <Elpida/Engine/Runner/BenchmarkRunner.hpp>
#include <Elpida/Engine/Configuration/Concrete/BenchmarkConfiguration.hpp>
#include <Utilities/BenchmarkLoader.hpp>
#include <json.hpp>
#include "Core/JsonResultFormatter.hpp"

#define NON_EXIT_CODE (-1)

using namespace Elpida;

struct RunConfiguration
{
	BenchmarkConfiguration benchmarkConfiguration;
	TaskAffinity affinity;
};

static void printVersion()
{
	std::cout << "Elpida Benchmark Executor: " << ELPIDA_VERSION << std::endl;
	std::cout << "Compiler: " << ELPIDA_COMPILER_NAME << " Version: " << ELPIDA_COMPILER_VERSION << std::endl;
}

static void printHelp()
{
	std::cout << "Elpida Benchmark Executor: " << ELPIDA_VERSION << std::endl;
	std::cout << "Usage: elpida-executor --library=LIBRARY_PATH --benchmark=BENCHMARK_UUID --config=BENCHMARK_CONFIG"
			  << std::endl;
	std::cout << "       -v, --version" << std::endl;
	std::cout << "           Prints the version and exits" << std::endl;
	std::cout << "       -h, --help" << std::endl;
	std::cout << "           Prints this help and exits" << std::endl;
	std::cout << "       -l=LIBRARY_PATH, --library=LIBRARY_PATH" << std::endl;
	std::cout << "           Sets the benchmark library path to load the benchmark from" << std::endl;
	std::cout << "       -b=BENCHMARK_UUID, --benchmark=BENCHMARK_UUID" << std::endl;
	std::cout << "           Sets the benchmark UUID to run" << std::endl;
	std::cout << "       -c=BENCHMARK_CONFIG, --config=BENCHMARK_CONFIG" << std::endl;
	std::cout << "           Sets the benchmark configuration from JSON object wrapped in \"" << std::endl;
	std::cout << "       -i=ITERATIONS, --iterations=ITERATIONS" << std::endl;
	std::cout << "           the number of iteration to run the benchmark" << std::endl;
	std::cout << "       -t=TIME, --time=TIME" << std::endl;
	std::cout << "           Sets the target time for benchmarks in milliseconds" << std::endl;
}

static int getNumber(long& number, const std::string& name)
{
	char* endPtr = nullptr;
	errno = 0;
	number = strtol(optarg, &endPtr, 10);

	if (errno != 0)
	{
		perror((name + " argument error").c_str());
		return EXIT_FAILURE;
	}

	if (optarg == endPtr)
	{
		std::cerr << name << " had no digits" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

static int
processArgumentsAndCheckIfWeMustExit(int argC, char* argV[], std::string& libraryPath, std::string& benchmarkUUID,
		std::string& benchmarkConfigJson, long& timeMs, long& iterations)
{
	enum Opts
	{
		Arg_Version,
		Arg_Help,
	};
	struct option options[] = {
			{ "version",    no_argument,       nullptr, 'v' },
			{ "help",       no_argument,       nullptr, 'h' },
			{ "library",    required_argument, nullptr, 'l' },
			{ "benchmark",  required_argument, nullptr, 'b' },
			{ "config",     required_argument, nullptr, 'c' },
			{ "time",       optional_argument, nullptr, 't' },
			{ "iterations", optional_argument, nullptr, 'i' },
			{ nullptr, 0,                      nullptr, 0 }
	};

	int option_index = 0;
	int c = 0;
	while ((c = getopt_long(argC, argV, "vhl:b:c:t:", options, &option_index)) != -1)
	{
		switch (c)
		{
		case 'v':
			printVersion();
			return EXIT_SUCCESS;
		case 'h':
			printHelp();
			return EXIT_SUCCESS;
		case 'b':
			if (optarg != nullptr)
			{
				benchmarkUUID = optarg;
			}
			else
			{
				std::cerr << "--benchmark requires UUID eg: --benchmark=\"328ffa99-0579-43c1-ba1a-396a425f6038\""
						  << std::endl;
				return EXIT_FAILURE;
			}
			break;
		case 'l':
			if (optarg != nullptr)
			{
				libraryPath = optarg;
			}
			else
			{
				std::cerr << "--library requires a path eg: --library=\"./Benchmarks/libImageBenchmarks.so\""
						  << std::endl;
				return EXIT_FAILURE;
			}
			break;
		case 'c':
			if (optarg != nullptr)
			{
				benchmarkConfigJson = optarg;
			}
			else
			{
				std::cerr << R"(--config requires a json object eg: --config="{"config": {}}")" << std::endl;
				return EXIT_FAILURE;
			}
			break;
		case 'i':
			if (optarg != nullptr)
			{
				if (getNumber(iterations, "iterations") == EXIT_FAILURE)
				{
					return EXIT_FAILURE;
				}
			}
			else
			{
				std::cerr << R"(--iterations requires a number of iterations eg: --iterations=100)" << std::endl;
				return EXIT_FAILURE;
			}
			break;
		case 't':
			if (optarg != nullptr)
			{
				if (getNumber(timeMs, "time") == EXIT_FAILURE)
				{
					return EXIT_FAILURE;
				}
			}
			else
			{
				std::cerr << R"(--time requires a number of milliseconds eg: --time=100)" << std::endl;
				return EXIT_FAILURE;
			}
			break;
		case '?':
			return EXIT_FAILURE;
		default:
			break;
		}
	}

	if (libraryPath.empty())
	{
		printHelp();
		std::cerr << R"(required argument: --library="...")" << std::endl;
		return EXIT_FAILURE;
	}

	if (benchmarkUUID.empty())
	{
		printHelp();
		std::cerr << R"(required argument: --benchmark="...")" << std::endl;
		return EXIT_FAILURE;
	}

	if (benchmarkConfigJson.empty())
	{
		printHelp();
		std::cerr << R"(required argument: --config="...")" << std::endl;
		return EXIT_FAILURE;
	}

	return NON_EXIT_CODE;
}

static TaskAffinity getAffinity(const nlohmann::json& root, const SystemTopology& topology)
{
	auto& affinity = root["affinity"];
	if (!affinity.is_array())
	{
		throw std::runtime_error("affinity must be an array of numbers");
	}

	if (affinity.empty())
	{
		throw std::runtime_error("affinity must not be empty");
	}

	std::unordered_map<long, const ProcessorNode*> availableProcessors;
	for (auto processor: topology.getAllProcessors())
	{
		availableProcessors.emplace(processor->getOsIndex(), processor);
	}

	std::vector<const ProcessorNode*> processors;
	for (const auto& value: affinity)
	{
		if (!value.is_number_integer())
		{
			throw std::runtime_error("affinity must contain only integers");
		}


		auto osIndex = value.get<long>();
		auto itr = availableProcessors.find(osIndex);
		if (itr == availableProcessors.end())
		{
			throw std::runtime_error(Vu::Cs("No available processor with os index: ", osIndex));
		}
		processors.push_back(itr->second);
	}

	return TaskAffinity(processors);
}


static BenchmarkConfiguration getTaskConfigurations(const nlohmann::json& root, const Benchmark& benchmark)
{
	auto& tasksConfigsJ = root["tasksConfig"];
	if (!tasksConfigsJ.is_array())
	{
		throw std::runtime_error("configuration requires an member 'tasksConfig' with the task config array");
	}

	auto& taskBuilders = benchmark.getTaskBuilders();

	if (tasksConfigsJ.size() != taskBuilders.size())
	{
		throw std::runtime_error(
				Vu::Cs("configuration requires an member 'tasksConfig' must have a size the same of the benchmarks tasks (",
						taskBuilders.size(), ") but has (", tasksConfigsJ.size(), ")"));
	}

	BenchmarkConfiguration benchmarkConfiguration(benchmark);
	std::size_t i = 0;
	for (auto& taskBuilder: taskBuilders)
	{
		auto& taskConfigJ = tasksConfigsJ[i];
		if (!tasksConfigsJ.is_object())
		{
			throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "]' should be 'object'"));
		}

		TaskConfiguration config(taskBuilder.getTaskSpecification(), taskBuilder);
		if (taskBuilder.canBeDisabled())
		{
			config.setEnabled(taskConfigJ["enabled"].get<bool>());
		}

		auto& optionsJ = tasksConfigsJ["options"];
		if (!optionsJ.is_object())
		{
			throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options' should be 'object'"));
		}

		for (auto& configSpec: taskBuilder
				.getTaskSpecification()
				.getConfigurationSpecifications())
		{
			auto& optionJ = optionsJ[configSpec->getName()];

			auto option = configSpec->createDefault();
			switch (configSpec->getType())
			{
			case ConfigurationType::Type::Bool:
				if (!optionJ.is_boolean())
				{
					throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options[\"", configSpec->getName(),
							"\"]" "' should be 'boolean'"));
				}
				option->as<ConfigurationValue<ConfigurationType::Bool>>().setValue(optionJ.get<bool>());
				break;
			case ConfigurationType::Type::Int:
				if (!optionJ.is_number_integer())
				{
					throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options[\"", configSpec->getName(),
							"\"]" "' should be 'integer'"));
				}
				option->as<ConfigurationValue<ConfigurationType::Int>>().setValue(optionJ.get<int>());
				break;
			case ConfigurationType::Type::UnsignedInt:
				if (!optionJ.is_number_unsigned())
				{
					throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options[\"", configSpec->getName(),
							"\"]" "' should be 'unsigned integer'"));
				}
				option->as<ConfigurationValue<ConfigurationType::UnsignedInt>>().setValue(
						optionJ.get<ConfigurationType::UnsignedInt>());
				break;
			case ConfigurationType::Type::Float:
				if (!optionJ.is_number_float())
				{
					throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options[\"", configSpec->getName(),
							"\"]" "' should be 'float'"));
				}
				option->as<ConfigurationValue<ConfigurationType::Float>>().setValue(
						optionJ.get<ConfigurationType::Float>());
				break;
			case ConfigurationType::Type::String:
				if (!optionJ.is_string())
				{
					throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options[\"", configSpec->getName(),
							"\"]" "' should be 'string'"));
				}
				option->as<ConfigurationValue<ConfigurationType::FolderPath>>().setValue(
						optionJ.get<ConfigurationType::FolderPath>());
				break;
			case ConfigurationType::Type::FilePath:
				if (!optionJ.is_string())
				{
					throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options[\"", configSpec->getName(),
							"\"]" "' should be 'string'"));
				}
				option->as<ConfigurationValue<ConfigurationType::FolderPath>>().setValue(
						optionJ.get<ConfigurationType::FolderPath>());
				break;
			case ConfigurationType::Type::FolderPath:
				if (!optionJ.is_string())
				{
					throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options[\"", configSpec->getName(),
							"\"]" "' should be 'string'"));
				}
				option->as<ConfigurationValue<ConfigurationType::FolderPath>>().setValue(
						optionJ.get<ConfigurationType::FolderPath>());
				break;
			case ConfigurationType::Type::Function:
			case ConfigurationType::Type::Custom:
				break;
			}
			config.setConfiguration(configSpec->getName(), std::move(option));
		}

		benchmarkConfiguration.addConfiguration(taskBuilder, std::move(config));
		i++;
	}

	return benchmarkConfiguration;
}

static RunConfiguration parseConfig(const std::string& data, const Benchmark& benchmark, const SystemTopology& topology)
{
	auto root = nlohmann::json::parse(data);

	return {
			getTaskConfigurations(root, benchmark),
			getAffinity(root, topology)
	};
}

int main(int argC, char** argV)
{
	std::string libraryPath;
	std::string benchmarkUUID;
	std::string benchmarkConfig;
	long time;
	long iterations;

	auto exitCode = processArgumentsAndCheckIfWeMustExit(argC, argV, libraryPath, benchmarkUUID, benchmarkConfig, time,
			iterations);
	if (exitCode != NON_EXIT_CODE)
	{
		return exitCode;
	}

	TimingInfo timingInfo;

	if (time <= 0)
	{
		std::cerr << "Target time must be greater than zero (0)" << std::endl;
		return EXIT_FAILURE;
	}

	if (iterations <= 0)
	{
		std::cerr << "iterations must be greater than zero (0)" << std::endl;
		return EXIT_FAILURE;
	}

	timingInfo.setTargetTime(MilliSeconds(time));

	const CpuInfo& cpuInfo = CpuInfo::get();
	SystemTopology topology;
	MemoryInfo memoryInfo;
	OsInfo osInfo;
	Logger logger;
	ServiceProvider serviceProvider(cpuInfo, memoryInfo, topology, osInfo, timingInfo, logger);

	BenchmarkLoader loader(serviceProvider);

	const std::vector<std::unique_ptr<Benchmark>>* benchmarks;
	try
	{
		benchmarks = &loader.load(libraryPath);
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Failed to load benchmarkIterator: " << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	auto benchmarkIterator = std::find_if(benchmarks->begin(), benchmarks->end(), [&benchmarkUUID](auto& a)
	{
		return a->getUuid() == benchmarkUUID;
	});

	if (benchmarkIterator == benchmarks->end())
	{
		std::cerr << "Could not find benchmarkIterator with UUID: " << benchmarkUUID << " in " << libraryPath
				  << std::endl;
		return EXIT_FAILURE;
	}

	auto& benchmark = **benchmarkIterator;

	OffThreadExecutor taskRunnerThread;
	BenchmarkRunner runner(serviceProvider);

	std::vector<BenchmarkRunRequest> requests;

	auto config = parseConfig(benchmarkConfig, benchmark, topology);

	for (long i = 0; i < iterations; ++i)
	{
		requests.emplace_back(benchmark, config.benchmarkConfiguration);
	}
	auto result = runner.runBenchmarks(requests, config.affinity);

	JsonResultFormatter resultFormatter(topology, cpuInfo, osInfo, memoryInfo, timingInfo);

	std::cout << resultFormatter.serialize(result) << std::endl;

	return EXIT_SUCCESS;
}