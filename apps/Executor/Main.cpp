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

#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Platform/BenchmarkGroupModule.hpp"
#include "Elpida/Platform/TopologyLoader.hpp"

#include "ArgumentsValidator.hpp"
#include "Elpida/Platform/NumaAllocator.hpp"

#define NON_EXIT_CODE (-1)

using namespace Elpida;

static void printVersion()
{
	std::cout << "Elpida Benchmark Executor: " << ELPIDA_VERSION << std::endl;
	std::cout << "Compiler: " << ELPIDA_COMPILER_NAME << " Version: " << ELPIDA_COMPILER_VERSION << std::endl;
}

static void printHelp()
{
	std::cout << R"("Elpida Benchmark Executor: )" << ELPIDA_VERSION << std::endl;
	std::cout
		<< R"(Example usage: elpida-executor --module="./dir/benchmark.so" --index=0 --affinity=0,1,5,32 --format=json --config="Config A" --config="Config B" ...)"
		<< std::endl;
	std::cout << R"("       -v, --version)" << std::endl;
	std::cout << R"("           Prints the version and exits)" << std::endl;
	std::cout << R"("       -h, --help)" << std::endl;
	std::cout << R"("           Prints this help and exits)" << std::endl;
	std::cout << R"("       -m="MODULE_PATH", --module="MODULE_PATH")" << std::endl;
	std::cout << R"("           The library that contains the benchmark group)" << std::endl;
	std::cout << R"("       -i=BENCHMARK_INDEX, --index=BENCHMARK_INDEX)" << std::endl;
	std::cout << R"("           The index of the benchmark in the benchmark group)" << std::endl;
	std::cout << R"("       -a=AFFINITY, --affinity=AFFINITY)" << std::endl;
	std::cout << R"("           The comma separated processors to use.)" << std::endl;
	std::cout << R"("       -f=FORMAT, --format=FORMAT)" << std::endl;
	std::cout << R"("           The result output format. Accepted values (default, json))" << std::endl;
	std::cout << R"("       -c="CONFIG_VALUE", --config="CONFIG_VALUE")" << std::endl;
	std::cout << R"("           Sets a configuration. Successive configurations are appended in the order defined)"
			  << std::endl;
}

static const char* GetValueOrDefault(const char* value)
{
	return value ? value : "";
}

static int ReadArgumentsAndCheckIfWeMustExit(
	int argC,
	char* argV[],
	String& modulePath,
	String& benchmarkIndex,
	String& affinity,
	String& format,
	Vector<String>& taskConfigurations)
{
	struct option options[] = {
		{ "version",  no_argument,       nullptr, 'v' },
		{ "help",     no_argument,       nullptr, 'h' },
		{ "module",   required_argument, nullptr, 'm' },
		{ "index",    required_argument, nullptr, 'i' },
		{ "affinity", required_argument, nullptr, 'a' },
		{ "format",   required_argument, nullptr, 'f' },
		{ "config",   required_argument, nullptr, 'c' },
		{ nullptr, 0,                    nullptr, 0 }
	};

	int option_index = 0;
	int c;
	while ((c = getopt_long(argC, argV, "vhm:i:a:f:c:", options, &option_index)) != -1)
	{
		switch (c)
		{
		case 'v':
			printVersion();
			return EXIT_SUCCESS;
		case 'h':
			printHelp();
			return EXIT_SUCCESS;
		case 'm':
			modulePath = GetValueOrDefault(optarg);
			break;
		case 'i':
			benchmarkIndex = GetValueOrDefault(optarg);
			break;
		case 'a':
			affinity = GetValueOrDefault(optarg);
			break;
		case 'f':
			format = GetValueOrDefault(optarg);
			break;
		case 'c':
			taskConfigurations.emplace_back(GetValueOrDefault(optarg));
			break;
		case '?':
			return EXIT_FAILURE;
		default:
			break;
		}
	}

	return NON_EXIT_CODE;
}

int main(int argC, char** argV)
{

	Vector<String> taskConfigurations;
	String modulePath;
	String benchmarkIndex;
	String affinity;
	String format;

	auto exitCode = ReadArgumentsAndCheckIfWeMustExit(argC, argV, modulePath, benchmarkIndex, affinity, format, taskConfigurations);
	if (exitCode != NON_EXIT_CODE)
	{
		return exitCode;
	}

	try
	{
		ArgumentsValidator validator(modulePath, benchmarkIndex, affinity, format);

		BenchmarkGroupModule module(modulePath);

		auto& benchmark = module.GetBenchmarkGroup().GetBenchmarks().at(validator.GetBenchmarkIndex());

		auto config = benchmark->GetRequiredConfiguration();
		validator.ValidateAndAssignConfiguration(taskConfigurations, config);

		EnvironmentInfo environmentInfo(OverheadsInfo(), TopologyLoader::LoadTopology(), std::make_unique<NumaAllocator>());

		auto targetProcessors = validator.ValidateAndGetProcessingUnits(environmentInfo.GetTopologyInfo());

		auto result = benchmark->Run(targetProcessors, config, environmentInfo);

		std::cout << validator.GetResultFormatter().ConvertToString(result) << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}