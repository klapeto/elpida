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
	std::cout << "       -c=BENCHMARK_CONFIG, --config=BENCHMARK_CONFIG" << std::endl;
	std::cout << "           Sets the benchmark configuration from JSON object wrapped in \"" << std::endl;
}

static int
processArgumentsAndCheckIfWeMustExit(int argC, char* argV[], std::string& benchmarkConfigJson)
{
	enum Opts
	{
		Arg_Version,
		Arg_Help,
	};
	struct option options[] = {
			{ "version", no_argument,       nullptr, 'v' },
			{ "help",    no_argument,       nullptr, 'h' },
			{ "config",  required_argument, nullptr, 'c' },
			{ nullptr, 0,                   nullptr, 0 }
	};

	int option_index = 0;
	int c;
	while ((c = getopt_long(argC, argV, "vhc:", options, &option_index)) != -1)
	{
		switch (c)
		{
		case 'v':
			printVersion();
			return EXIT_SUCCESS;
		case 'h':
			printHelp();
			return EXIT_SUCCESS;
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
		case '?':
			return EXIT_FAILURE;
		default:
			break;
		}
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


static BenchmarkConfiguration getBenchmarkConfiguration(const nlohmann::json& root, const Benchmark& benchmark)
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
		if (!taskConfigJ.is_object())
		{
			throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "]' should be 'object'"));
		}

		TaskConfiguration config = taskBuilder.getDefaultConfiguration();
		if (taskBuilder.canBeDisabled())
		{
			config.setEnabled(taskConfigJ["enabled"].get<bool>());
		}

		auto& optionsJ = taskConfigJ["options"];
		if (!optionsJ.is_object())
		{
			throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options' should be 'object'"));
		}

		for (auto& configSpec: taskBuilder
				.getTaskSpecification()
				.getConfigurationSpecifications())
		{
			auto configName = configSpec->getName();
			if (config.getConfiguration(configName).has_value())
			{
				continue;
			}

			auto option = configSpec->createDefault();
			switch (configSpec->getType())
			{
			case ConfigurationType::Type::Bool:
			{
				auto& optionJ = optionsJ[configName];
				if (!optionJ.is_boolean())
				{
					throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options[\"", configName, "\"]" "' should be 'boolean'"));
				}
				option->as<ConfigurationValue<ConfigurationType::Bool>>().setValue(optionJ.get<bool>());
			}
				break;
			case ConfigurationType::Type::Int:
			{
				auto& optionJ = optionsJ[configName];
				if (!optionJ.is_number_integer())
				{
					throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options[\"", configName,
							"\"]" "' should be 'integer'"));
				}
				option->as<ConfigurationValue<ConfigurationType::Int>>().setValue(optionJ.get<int>());
			}
				break;
			case ConfigurationType::Type::UnsignedInt:
			{
				auto& optionJ = optionsJ[configName];
				if (!optionJ.is_number_unsigned())
				{
					throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options[\"", configName,
							"\"]" "' should be 'unsigned integer'"));
				}
				option->as<ConfigurationValue<ConfigurationType::UnsignedInt>>().setValue(
						optionJ.get<ConfigurationType::UnsignedInt>());
			}
				break;
			case ConfigurationType::Type::Float:
			{
				auto& optionJ = optionsJ[configName];
				if (!optionJ.is_number_float())
				{
					throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options[\"", configName,
							"\"]" "' should be 'float'"));
				}
				option->as<ConfigurationValue<ConfigurationType::Float>>().setValue(
						optionJ.get<ConfigurationType::Float>());
			}
				break;
			case ConfigurationType::Type::String:
			{
				auto& optionJ = optionsJ[configName];
				if (!optionJ.is_string())
				{
					throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options[\"", configName,
							"\"]" "' should be 'string'"));
				}
				option->as<ConfigurationValue<ConfigurationType::FolderPath>>().setValue(
						optionJ.get<ConfigurationType::FolderPath>());
			}
				break;
			case ConfigurationType::Type::FilePath:
			{
				auto& optionJ = optionsJ[configName];
				if (!optionJ.is_string())
				{
					throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options[\"", configName,
							"\"]" "' should be 'string'"));
				}
				option->as<ConfigurationValue<ConfigurationType::FolderPath>>().setValue(
						optionJ.get<ConfigurationType::FolderPath>());
			}
				break;
			case ConfigurationType::Type::FolderPath:
			{
				auto& optionJ = optionsJ[configName];
				if (!optionJ.is_string())
				{
					throw std::runtime_error(Vu::Cs("'tasksConfig[", i, "].options[\"", configName,
							"\"]" "' should be 'string'"));
				}
				option->as<ConfigurationValue<ConfigurationType::FolderPath>>().setValue(
						optionJ.get<ConfigurationType::FolderPath>());
			}
				break;
			case ConfigurationType::Type::Function:
			case ConfigurationType::Type::Custom:
				break;
			}
			config.setConfiguration(configName, std::move(option));
		}

		benchmarkConfiguration.addConfiguration(taskBuilder, std::move(config));
		i++;
	}

	return benchmarkConfiguration;
}

static long getTargetTime(const nlohmann::json& root)
{
	auto itr = root.find("targetTimeMs");
	if (itr == root.end())
	{
		throw std::runtime_error("'targetTimeMs' should be exist");
	}

	auto& targetTimeJ = *itr;
	if (!targetTimeJ.is_number_unsigned())
	{
		throw std::runtime_error("'targetTimeMs' should be 'unsigned integer'");
	}

	auto value = targetTimeJ.get<long>();

	if (value <= 0)
	{
		throw std::runtime_error("'targetTimeMs' should be greater than zero (0)");
	}

	return value;
}

static std::string getLibraryPath(const nlohmann::json& root)
{
	auto& libraryPathJ = root["libraryPath"];
	if (!libraryPathJ.is_string())
	{
		throw std::runtime_error("'libraryPath' should be 'string'");
	}

	if (libraryPathJ.empty())
	{
		throw std::runtime_error("'libraryPath' not be empty");
	}

	return libraryPathJ.get<std::string>();
}

static std::string getBenchmarkUuid(const nlohmann::json& root)
{
	auto& benchmarkUuidJ = root["benchmarkUUID"];
	if (!benchmarkUuidJ.is_string())
	{
		throw std::runtime_error("'benchmarkUUID' should be 'string'");
	}

	if (benchmarkUuidJ.empty())
	{
		throw std::runtime_error("'benchmarkUUID' not be empty");
	}

	return benchmarkUuidJ.get<std::string>();
}

static long getIterations(const nlohmann::json& root)
{
	auto& iterationsJ = root["iterations"];
	if (!iterationsJ.is_number_unsigned())
	{
		throw std::runtime_error("'iterations' should be 'unsigned integer'");
	}

	auto value = iterationsJ.get<long>();

	if (value <= 0)
	{
		throw std::runtime_error("'iterations' should be greater than zero (0)");
	}

	return value;
}

int main(int argC, char** argV)
{
	std::string benchmarkConfig;

	auto exitCode = processArgumentsAndCheckIfWeMustExit(argC, argV, benchmarkConfig);
	if (exitCode != NON_EXIT_CODE)
	{
		return exitCode;
	}

	const CpuInfo& cpuInfo = CpuInfo::get();
	SystemTopology topology;
	MemoryInfo memoryInfo;
	OsInfo osInfo;
	Logger logger;
	TimingInfo timingInfo;
	ServiceProvider serviceProvider(cpuInfo, memoryInfo, topology, osInfo, timingInfo, logger);
	BenchmarkLoader loader(serviceProvider);

	try
	{
		auto configRootJ = nlohmann::json::parse(benchmarkConfig);

		auto libraryPath = getLibraryPath(configRootJ);
		auto benchmarkUUID = getBenchmarkUuid(configRootJ);
		auto targetTimeMs = getTargetTime(configRootJ);
		auto iterations = getIterations(configRootJ);

		timingInfo.setTargetTime(MilliSeconds(targetTimeMs));

		auto& benchmarks = loader.load(getLibraryPath(configRootJ));

		auto benchmarkIterator = std::find_if(benchmarks.begin(), benchmarks.end(), [&benchmarkUUID](auto& a)
		{
			return a->getUuid() == benchmarkUUID;
		});

		if (benchmarkIterator == benchmarks.end())
		{
			throw std::runtime_error(
					Vu::Cs("Could not find benchmarkIterator with UUID: '", benchmarkUUID, "' in '", libraryPath, "'"));
		}

		auto& benchmark = **benchmarkIterator;

		OffThreadExecutor taskRunnerThread;
		BenchmarkRunner runner(serviceProvider);

		std::vector<BenchmarkRunRequest> requests;

		auto benchmarkConfiguration = getBenchmarkConfiguration(configRootJ, benchmark);
		auto affinity = getAffinity(configRootJ, topology);

		for (long i = 0; i < iterations; ++i)
		{
			requests.emplace_back(benchmark, benchmarkConfiguration);
		}

		auto result = runner.runBenchmarks(requests, affinity);

		JsonResultFormatter resultFormatter(topology, cpuInfo, osInfo, memoryInfo, timingInfo);

		std::cout << resultFormatter.serializeNoMetadata(result) << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Fail: " << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}