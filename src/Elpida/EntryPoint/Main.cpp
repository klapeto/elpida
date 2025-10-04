/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 10/12/2022.
//

#include <cstdlib>
#include <iostream>
#include <thread>

#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Core/AllocatorFactory.hpp"
#include "Elpida/Core/DefaultAllocatorFactory.hpp"
#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/TimingInfo.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Platform/NumaAllocatorFactory.hpp"
#include "Elpida/Platform/TopologyLoader.hpp"

#include "Elpida/Platform/OsInfoLoader.hpp"
#include "Elpida/Platform/CpuInfoLoader.hpp"
#include "Elpida/Platform/MemoryInfoLoader.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "Elpida/EntryPoint/ArgumentsHelper.hpp"

#include "json.hpp"

#include "Elpida/Core/ModuleExports.hpp"

using namespace nlohmann;
using namespace Elpida;

static nlohmann::json Serialize(const Benchmark& benchmark)
{
	json jBenchmark;
	auto info = benchmark.GetInfo();

	jBenchmark["name"] = info.GetName();
	jBenchmark["description"] = info.GetDescription();
	jBenchmark["resultUnit"] = info.GetResultUnit();
	jBenchmark["resultType"] = info.GetResultType();

	json jRequiredConfiguration = json::array();

	for (auto& config: benchmark.GetRequiredConfiguration())
	{
		json jConfig;

		jConfig["name"] = config.GetName();
		jConfig["type"] = config.GetType();
		jConfig["defaultValue"] = config.GetValue();

		jRequiredConfiguration.push_back(std::move(jConfig));
	}

	jBenchmark["requiredConfiguration"] = std::move(jRequiredConfiguration);

	return jBenchmark;
}

static nlohmann::json Serialize(const BenchmarkGroup& benchmarkGroup)
{
	json jBenchmarkGroup;

	jBenchmarkGroup["name"] = benchmarkGroup.GetName();

	json benchmarkArray = json::array();

	auto& benchmarks = benchmarkGroup.GetBenchmarks();
	for (const auto & benchmark : benchmarks)
	{
		benchmarkArray.push_back(Serialize(*benchmark));
	}

	jBenchmarkGroup["benchmarks"] = std::move(benchmarkArray);

	return jBenchmarkGroup;
}

static json SerializeBenchmarkGroup(const BenchmarkGroup& group)
{
	auto value = Serialize(group);

	std::size_t i = 0;
	for (auto& benchmark : value["benchmarks"])
	{
		benchmark["index"] = i++;
	}
	return value;
}

static Vector<Ref<const ProcessingUnitNode>>
ValidateAndGetProcessingUnits(const Vector<unsigned int>& affinity, const TopologyInfo& topologyInfo)
{
	if (affinity.empty()) return topologyInfo.GetAllProcessingUnits();

	Vector<Ref<const ProcessingUnitNode>> returnVector;
	returnVector.reserve(affinity.size());

	auto& processors = topologyInfo.GetAllProcessingUnits();
	for (auto index : affinity)
	{
		bool found = false;
		for (auto& processor : processors)
		{
			if (processor.get().GetOsIndex().value() == index)
			{
				returnVector.emplace_back(processor.get());
				found = true;
				break;
			}
		}

		if (!found)
		{
			throw ElpidaException("No processor with id ", index, " was found");
		}
	}

	return returnVector;
}

static void
ValidateAndAssignConfiguration(const Vector<String>& configurationValues, Vector<TaskConfiguration>& taskConfigurations)
{
	if (configurationValues.size() != taskConfigurations.size())
	{
		throw ElpidaException("benchmark required ",
				taskConfigurations.size(),
				" configurations but were provided ",
				configurationValues.size());
	}

	for (Size i = 0; i < taskConfigurations.size(); ++i)
	{
		taskConfigurations[i].Parse(configurationValues[i]);
	}
}

ELPIDA_CREATE_BENCHMARK_GROUP_FUNC();

int main(int argC, char** argV)
{
	try
	{
		OsUtilities::ConvertArgumentsToUTF8(argC, argV);

		std::locale::global(std::locale::classic());
		ArgumentsHelper helper;

		{
			std::string returnText;
			auto success = helper.ParseAndGetExitText(argC, argV, returnText);
			if (!returnText.empty())
			{
				std::cout << returnText << std::endl;
				return success ? EXIT_SUCCESS : EXIT_FAILURE;
			}

			if (success && helper.GetDumpInfo())
			{
				auto group = CreateBenchmarkGroup();

				std::cout << SerializeBenchmarkGroup(*group).dump();
				return EXIT_SUCCESS;
			}
		}

		auto group = CreateBenchmarkGroup();

		auto& benchmark = group->GetBenchmarks()
				.at(helper.GetBenchmarkIndex());

		auto config = benchmark->GetRequiredConfiguration();
		ValidateAndAssignConfiguration(helper.GetConfigurationValues(), config);

		EnvironmentInfo environmentInfo(
				CpuInfoLoader::Load(),
				MemoryInfoLoader::Load(),
				OsInfoLoader::Load(),
				TopologyLoader::LoadTopology(),
				TimingInfo(Seconds(helper.GetNowOverhead()),
						Seconds(helper.GetLoopOverhead()),
						0));

		auto targetProcessors = ValidateAndGetProcessingUnits(helper.GetAffinity(), environmentInfo.GetTopologyInfo());

		if (!helper.GetAffinity().empty())
		{
			environmentInfo.GetTopologyInfo().PinProcessToProcessors(targetProcessors);
		}

		auto context = BenchmarkRunContext(targetProcessors, config,
				helper.GetNumaAware() ?
				UniquePtr<AllocatorFactory>(new NumaAllocatorFactory())
									  : UniquePtr<AllocatorFactory>(new DefaultAllocatorFactory()),
				environmentInfo,
				helper.GetConcurrencyMode(),
				helper.GetPinThreads(), Seconds(helper.GetMicroTaskDuration()));

		auto result = benchmark->Run(context);

		auto output = helper.GetResultFormatter().ConvertToString(result, *benchmark.get());
		std::cout << output << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}