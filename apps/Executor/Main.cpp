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

#define NON_EXIT_CODE (-1)

using namespace Elpida;

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

static TaskConfiguration getTaskConfiguration(
		const TaskBuilder& taskBuilder,
		const nlohmann::json& taskConfigJ)
{
	auto& taskSpecification = taskBuilder.getTaskSpecification();
	TaskConfiguration taskConfiguration(taskSpecification, taskBuilder);
	auto& configurationSpecifications = taskSpecification.getConfigurationSpecifications();
	auto& configValuesJ = taskConfigJ["values"];
	for (auto& configSpec: configurationSpecifications)
	{
		auto config = configSpec->createDefault();
		switch (configSpec->getType())
		{
		case ConfigurationType::Type::String:
			if (!taskConfigJ[configSpec->getName()].is_string())
			{
				throw std::runtime_error(
						Vu::Cs("Config of '", taskSpecification.getName(), "' of task '", configSpec->getName(),
								"' requires the configuration of type 'string'"));
			}
			config->as<ConfigurationValue<ConfigurationType::String>>().setValue(
					taskConfigJ.get<ConfigurationType::String>());
			break;
		case ConfigurationType::Type::Bool:
			if (!taskConfigJ[configSpec->getName()].is_boolean())
			{
				throw std::runtime_error(
						Vu::Cs("Config of '", taskSpecification.getName(), "' of task '", configSpec->getName(),
								"' requires the configuration of type 'bool'"));
			}
			config->as<ConfigurationValue<ConfigurationType::Bool>>().setValue(
					taskConfigJ.get<ConfigurationType::Bool>());
			break;
		case ConfigurationType::Type::Int:
			if (!taskConfigJ[configSpec->getName()].is_number_integer())
			{
				throw std::runtime_error(
						Vu::Cs("Config of '", taskSpecification.getName(), "' of task '", configSpec->getName(),
								"' requires the configuration of type 'int'"));
			}
			config->as<ConfigurationValue<ConfigurationType::Int>>().setValue(
					taskConfigJ.get<ConfigurationType::Int>());
			break;
		case ConfigurationType::Type::UnsignedInt:
			if (!taskConfigJ[configSpec->getName()].is_number_unsigned())
			{
				throw std::runtime_error(
						Vu::Cs("Config of '", taskSpecification.getName(), "' of task '", configSpec->getName(),
								"' requires the configuration of type 'unsigned int'"));
			}
			config->as<ConfigurationValue<ConfigurationType::UnsignedInt>>().setValue(
					taskConfigJ.get<ConfigurationType::UnsignedInt>());
			break;
		case ConfigurationType::Type::Float:
			if (!taskConfigJ[configSpec->getName()].is_number_float())
			{
				throw std::runtime_error(
						Vu::Cs("Config of '", taskSpecification.getName(), "' of task '", configSpec->getName(),
								"' requires the configuration of type 'float'"));
			}
			config->as<ConfigurationValue<ConfigurationType::Float>>().setValue(
					taskConfigJ.get<ConfigurationType::Float>());
			break;
		case ConfigurationType::Type::FilePath:
			if (!taskConfigJ[configSpec->getName()].is_string())
			{
				throw std::runtime_error(
						Vu::Cs("Config of '", taskSpecification.getName(), "' of task '", configSpec->getName(),
								"' requires the configuration of type 'string'"));
			}
			config->as<ConfigurationValue<ConfigurationType::FilePath>>().setValue(
					taskConfigJ.get<ConfigurationType::FilePath>());
			break;
		case ConfigurationType::Type::FolderPath:
			if (!taskConfigJ[configSpec->getName()].is_string())
			{
				throw std::runtime_error(
						Vu::Cs("Config of '", taskSpecification.getName(), "' of task '", configSpec->getName(),
								"' requires the configuration of type 'string'"));
			}
			config->as<ConfigurationValue<ConfigurationType::FolderPath>>().setValue(
					taskConfigJ.get<ConfigurationType::FolderPath>());
			break;
		case ConfigurationType::Type::Function:
			throw std::runtime_error("Configuration of type 'function' is not supported'");
		case ConfigurationType::Type::Custom:
			throw std::runtime_error("Configuration of type 'custom' is not supported'");
		}
		taskConfiguration.setConfiguration(configSpec->getName(), std::move(config));
	}

	if (taskBuilder.canBeDisabled())
	{
		auto& enabled = taskConfigJ["enabled"];
		taskConfiguration.setEnabled(enabled.get<bool>());
	}

	return taskConfiguration;
}

static std::vector<long> getAffinity(const nlohmann::json& root)
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

	std::vector<long> returnVales;

	for (const auto& value: affinity)
	{
		if (!value.is_number_integer())
		{
			throw std::runtime_error("affinity must contain only integers");
		}

		returnVales.push_back(value.get<long>());
	}

	return returnVales;
}

static std::vector<TaskConfiguration> getTaskConfigurations(const Benchmark& benchmark, const nlohmann::json& root)
{
	auto& configArrayJ = root["taskConfigurations"];

	if (!configArrayJ.is_array())
	{
		throw std::runtime_error("'taskConfigurations' must be an array");
	}

	auto& builders = benchmark.getTaskBuilders();

	if (configArrayJ.size() != builders.size())
	{
		throw std::runtime_error(
				"'taskConfigurations' array must have the same size of the tasks amount of the benchmark");
	}

	std::vector<TaskConfiguration> taskConfigurations;
	taskConfigurations.reserve(builders.size());
	for (std::size_t i = 0; i < builders.size(); ++i)
	{
		auto& taskBuilder = builders[i];
		auto& configJ = configArrayJ[i];
		taskConfigurations.push_back(getTaskConfiguration(taskBuilder, configJ));
	}
	return taskConfigurations;
}

static void parseConfig(const std::string& data, const Benchmark& benchmark)
{
	auto root = nlohmann::json::parse(data);

	auto affinity = getAffinity(root);


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

	auto& benchmark = *benchmarkIterator;

	OffThreadExecutor taskRunnerThread;
	BenchmarkRunner runner(serviceProvider);

	std::vector<BenchmarkRunRequest> requests;


	parseConfig(benchmarkConfig);


	return EXIT_SUCCESS;
}