/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
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
#include <Elpida/Engine/DefaultBenchmarkScoreCalculator.hpp>
#include <Elpida/CommonTasks/AllocateMemory/AllocateMemorySpecification.hpp>
#include <Elpida/Engine/Task/TaskBuilder.hpp>
#include "Benchmarks/Memory/Read/MemoryReadSpecification.hpp"
#include "Benchmarks/Memory/Latency/MemoryReadLatencySpecification.hpp"

#include "Benchmarks/Memory/WorkingSetSizes.hpp"

Elpida::Benchmark* createMemoryReadBandwidth();
Elpida::Benchmark* createMemoryReadLatency();

extern "C" Elpida::BenchmarksContainerPlugin<Elpida::Benchmark>* createPlugin()
{
	using namespace Elpida;
	using Plugin = BenchmarksContainerPlugin<Elpida::Benchmark>;

	auto plugin = new Plugin();

	plugin->add(createMemoryReadBandwidth());
	plugin->add(createMemoryReadLatency());

	return plugin;
}

Elpida::Benchmark* createMemoryReadLatency()
{
	std::vector<Elpida::TaskBuilder*> tasksBuilders;

	for (auto size : Elpida::WorkingSetSizes::Values)
	{
		auto& allocateMemory = (new Elpida::TaskBuilder(*new Elpida::AllocateMemorySpecification))
			->shouldBeCountedOnResults(false)
			.canBeMultiThreaded(false)
			.canBeDisabled(false)
			.withFixedConfiguration(Elpida::AllocateMemorySpecification::Settings::MemorySize, Elpida::ConfigurationType::UnsignedInt(size));

		auto& memoryLatency = (new Elpida::TaskBuilder(*new Elpida::MemoryReadLatencySpecification))
			->shouldBeCountedOnResults(true)
			.canBeMultiThreaded(false)
			.canBeDisabled(false);

		tasksBuilders.push_back(&allocateMemory);
		tasksBuilders.push_back(&memoryLatency);
	}

	auto benchmark = new Elpida::Benchmark("Memory Read Latency", std::move(tasksBuilders), new Elpida::DefaultBenchmarkScoreCalculator());
	return benchmark;
}

Elpida::Benchmark* createMemoryReadBandwidth()
{
	auto& allocateMemory = (new Elpida::TaskBuilder(*new Elpida::AllocateMemorySpecification))
		->shouldBeCountedOnResults(false)
		.canBeMultiThreaded(false)
		.canBeDisabled(false)
		.withDefaultConfiguration(Elpida::AllocateMemorySpecification::Settings::MemorySize, Elpida::ConfigurationType::UnsignedInt(4096));

	auto& memoryRead = (new Elpida::TaskBuilder(*new Elpida::MemoryReadSpecification))
		->shouldBeCountedOnResults(true)
		.canBeMultiThreaded(true)
		.canBeDisabled(false);

	auto benchmark = new Elpida::Benchmark("Memory Read Bandwidth", {
		&allocateMemory,
		&memoryRead,
	}, new Elpida::DefaultBenchmarkScoreCalculator());
	return benchmark;
}

