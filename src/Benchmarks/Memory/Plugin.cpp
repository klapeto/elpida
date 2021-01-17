/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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

/*
 * Plugin.cpp
 *
 *  Created on: 18 Οκτ 2018
 *      Author: klapeto
 */

#include <Elpida/Utilities/Plugin/BenchmarksContainerPlugin.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Engine/Calculators/Benchmark/AccumulativeScoreCalculator.hpp>
#include <Elpida/CommonTasks/AllocateMemory/AllocateMemorySpecification.hpp>
#include <Elpida/Engine/Task/TaskBuilder.hpp>
#include <Elpida/Engine/Calculators/Benchmark/AverageScoreCalculator.hpp>
#include <Elpida/Engine/Calculators/Task/AverageTaskResultCalculator.hpp>
#include <memory>
#include "Benchmarks/Memory/Read/MemoryReadSpecification.hpp"
#include "Benchmarks/Memory/Latency/MemoryReadLatencySpecification.hpp"

#include "Benchmarks/Memory/WorkingSetSizes.hpp"

using namespace Elpida;

std::unique_ptr<Benchmark> createMemoryReadBandwidth(const std::shared_ptr<AllocateMemorySpecification>& allocateSpec);
std::unique_ptr<Benchmark> createMemoryReadLatency(const std::shared_ptr<AllocateMemorySpecification>& allocateSpec);

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL int32_t elpidaPluginAbiVersion()
{
	return ELPIDA_PLUGIN_ABI_VERSION;
}

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL void elpidaDestroyPlugin(Elpida::BenchmarksContainerPlugin<Elpida::Benchmark>* plugin)
{
	delete plugin;
}

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL Elpida::BenchmarksContainerPlugin<Elpida::Benchmark>* elpidaCreatePlugin()
{
	using Plugin = BenchmarksContainerPlugin<Benchmark>;

	auto plugin = new Plugin("Memory Benchmarks");

	auto allocate = std::make_shared<AllocateMemorySpecification>();

	plugin->add(createMemoryReadBandwidth(allocate));
	plugin->add(createMemoryReadLatency(allocate));

	return plugin;
}

std::unique_ptr<Benchmark> createMemoryReadLatency(const std::shared_ptr<AllocateMemorySpecification>& allocateSpec)
{
	auto benchmark = std::make_unique<Benchmark>("Memory Read Latency", std::make_shared<AverageScoreCalculator>("s"));

	auto calculator = std::make_shared<AverageTaskResultCalculator>();
	auto readLatency = std::make_shared<MemoryReadLatencySpecification>();

	for (auto size : WorkingSetSizes::Values)
	{
		benchmark->AddTask(allocateSpec)
			.shouldBeCountedOnResults(false)
			.canBeMultiThreaded(false)
			.canBeDisabled(false)
			.withFixedConfiguration(AllocateMemorySpecification::Settings::MemorySize,
				ConfigurationType::UnsignedInt(size));

		benchmark->AddTask(readLatency)
			.shouldBeCountedOnResults(true)
			.withIterationsToRun(2)
			.withTaskResultCalculator(calculator)
			.canBeMultiThreaded(false)
			.canBeDisabled(false);
	}

	return benchmark;
}

std::unique_ptr<Benchmark> createMemoryReadBandwidth(const std::shared_ptr<AllocateMemorySpecification>& allocateSpec)
{
	auto benchmark = std::make_unique<Benchmark>("Memory Read Bandwidth",
		std::make_shared<AccumulativeScoreCalculator>("B", ResultType::Throughput));

	benchmark->AddTask(allocateSpec)
		.shouldBeCountedOnResults(false)
		.canBeMultiThreaded(false)
		.canBeDisabled(false)
		.withDefaultConfiguration(AllocateMemorySpecification::Settings::MemorySize,
			ConfigurationType::UnsignedInt(4096));

	benchmark->AddTask<MemoryReadSpecification>()
		.shouldBeCountedOnResults(true)
		.withIterationsToRun(2)
		.canBeMultiThreaded(true)
		.canBeDisabled(false);

	return benchmark;
}

